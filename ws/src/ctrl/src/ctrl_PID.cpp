#include "ctrl_PID.h"
#include <chrono>                       // time utils
#include <string>
#include <memory>                       // smart pointers
#include <cmath>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>                     // for read() / write() / close() calls (necessary for CAN socket)
#include <net/if.h>                     // for looking up network interfaces (in our case for vcan0)

#include <linux/can.h>                  // CAN definitions
#include <linux/can/raw.h>              // CAN RAW protocol
#include <sys/ioctl.h>                  // get interface index
#include <sys/socket.h>                 // socket APIs

#include "rclcpp/rclcpp.hpp"            // ROS 2 C++ client library
#include "std_msgs/msg/string.hpp"      // for publishing/subscribing to text topics
#include "std_msgs/msg/float64.hpp"     // for float64 (== double) numbers

#include "eagle_msgs/msg/cone_array.hpp"
#include "eagle_msgs/msg/cone.hpp"

#include "geometry_msgs/msg/pose.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "nav_msgs/msg/path.hpp"
#include "geometry_msgs/msg/vector3.hpp"

using std::placeholders::_1;

using namespace std::chrono_literals;

VehicleStatus vehicle_status(0, 0, 0, 0);

class Publisher : public rclcpp::Node {
public:
    Publisher() : Node("PID"), count_(0) {     // Node naming and init count to 0
        publisher_ = this->create_publisher<std_msgs::msg::String>("PID", 10);     // Publisher initialized with string type and topic named "topic"
        initCAN();

        // Declare parameters
        this->declare_parameter<double>("RANGE_SIGHT");
        this->declare_parameter<double>("THROTTLE_A");
        this->declare_parameter<double>("BRAKE_A");
        this->declare_parameter<double>("KP");
        this->declare_parameter<double>("KI");
        this->declare_parameter<double>("KD");

        // Retrieve parameters from .yaml file
        RANGE_SIGHT = this->get_parameter("RANGE_SIGHT").as_double();       // in meters
        THROTTLE_A = this->get_parameter("THROTTLE_A").as_double();       // m/s^2
        BRAKE_A = this->get_parameter("BRAKE_A").as_double();         // m/s^2 (module)
        KP = this->get_parameter("KP").as_double();
        KI = this->get_parameter("KI").as_double();
        KD = this->get_parameter("KD").as_double();

        // Compute target speed
        target_speed = sqrt(2 * RANGE_SIGHT * BRAKE_A);
        //target_speed = 10;        // Used for PID tuning

        RCLCPP_INFO(this->get_logger(), "Parameter set: %f, %f, %f", KP, KI, KD);

        speed_timer_ = this->create_wall_timer(5ms, std::bind(&Publisher::readSpeed, this));

        plot_data_publisher_ = this->create_publisher<geometry_msgs::msg::Vector3>("plot", 10);     // Publisher for rqt_plot
        publish_data_timer_ = this->create_wall_timer(10ms, std::bind(&Publisher::send_data, this));

        // Timer for calculating throttle acceleration and brakes deceleration
        //check_timer_ = this->create_wall_timer(1ms, std::bind(&Publisher::check, this));

        // PID
        subscription_ = this->create_subscription<eagle_msgs::msg::ConeArray>(
              "perception/cones", 10, std::bind(&Publisher::PID_fsm, this, _1));

        // Kinematic controller
        /*subscription_ = this->create_subscription<eagle_msgs::msg::ConeArray>(
              "perception/cones", 10, std::bind(&Publisher::kinematic_controller_fsm, this, _1));*/

        // "Empirical" controller
        /*subscription_ = this->create_subscription<eagle_msgs::msg::ConeArray>(
              "perception/cones", 10, std::bind(&Publisher::empirical_controller_fsm, this, _1));*/

        send_timer_ = this->create_wall_timer(5ms, std::bind(&Publisher::send_AS_CMD, this));
    }

    void PID_fsm (const eagle_msgs::msg::ConeArray & msg) {
        /*
        error  = target_speed - measured_speed
        output = Kp * error + Ki * (sum of error over time) + Kd * (how fast the error is changing)
        */
        if (!sim_ready) {
            return;
        }

        switch (state) {
            case IDLE: {
                state = ACCELERATE;
                break;
            }
            case ACCELERATE: {
                // Update space from end of finish zone
                double space_from_finish = msg.cones.size() / 2 * 5 - 4;       // In meters (subtracting 4 meters for tolerance from last cone of finish zone)
                // Update space required to stop
                double braking_space_required = 0.5 * (vehicle_status.vehicle_speed * vehicle_status.vehicle_speed) / BRAKE_A;
                // If space required to stop is inside the finish zone -> stop PID and start braking
                if (msg.cones[msg.cones.size() - 1].color == 3 && space_from_finish <= braking_space_required) {
                    state = BRAKE;
                    vehicle_status.throttle = 0;
                    vehicle_status.brake = 1;
                    output = -1;        // Added for plotting
                } else {        // Let PID do its job
                    float error  = target_speed - vehicle_status.vehicle_speed;
                    double time = this->now().seconds();
                    if (first_iteration) {
                        output = KP * error + KI * error_sum;       // Avoid division by very small time
                        first_iteration = false;
                    } else {
                        error_sum += error * (time - prev_time);
                        output = KP * error + KI * error_sum + KD * (error - prev_error) / (time - prev_time);
                    }
                    //RCLCPP_INFO(this->get_logger(), "time_now = %f, prev_time = %f", time, prev_time);

                    if (time - prev_time == 0 && !first_iteration) {
                        RCLCPP_INFO(this->get_logger(), "(Time) Division by zero detected: time_now = %f, prev_time = %f", time, prev_time);
                    }

                    RCLCPP_INFO(this->get_logger(), "PID output: %f", output);

                    if (output >= 0) {
                        vehicle_status.throttle = std::min(output, 1.0f);      // Clamp to maximum physical range
                        vehicle_status.brake = 0;
                    } else {
                        output *= -1;
                        vehicle_status.throttle = 0;
                        vehicle_status.brake = std::min(output, 1.0f);          // Clamp to maximum physical range
                    }
                    prev_error = error;
                    prev_time = time;
                }
                break;
            }
            case BRAKE: {
                if (vehicle_status.vehicle_speed == 0) {
                    state = FINISH;
                }
                break;
            }
            case FINISH: {
                RCLCPP_INFO(this->get_logger(), "END");
                state = END;
                break;
            }
            case END: {
                break;
            }
        }

        update_AS_CMD();
    }

    void kinematic_controller_fsm (const eagle_msgs::msg::ConeArray & msg) {
        if (!sim_ready) {
            return;
        }
        switch (state) {
            case IDLE: {
                state = ACCELERATE;
                vehicle_status.throttle = 1;
                vehicle_status.brake = 0;
                break;
            }
            case ACCELERATE: {
                // Update space from end of finish zone
                double space_from_finish = msg.cones.size() / 2 * 5 - 4;       // In meters (subtracting 4 meters for tolerance from last cone of finish zone)
                // Update space required to stop
                double braking_space_required = 0.5 * (vehicle_status.vehicle_speed * vehicle_status.vehicle_speed) / BRAKE_A;
                if (msg.cones[msg.cones.size() - 1].color == 3 && space_from_finish <= braking_space_required) {
                    state = BRAKE;
                    vehicle_status.throttle = 0;
                    vehicle_status.brake = 1;
                    //RCLCPP_INFO(this->get_logger(), "Orange cone detected");
                }
                break;
            }
            case BRAKE: {
                if (vehicle_status.vehicle_speed == 0) {
                    state = FINISH;
                }
                break;
            }
            case FINISH: {
                RCLCPP_INFO(this->get_logger(), "END");
                state = END;
                break;
            }
            case END: {
                break;
            }
            default:
                RCLCPP_INFO(this->get_logger(), "ERROR: Switch case");
        }
        update_AS_CMD();
    }

    void empirical_controller_fsm (const eagle_msgs::msg::ConeArray & msg) {
        if (!sim_ready) {
            return;
        }
        switch (state) {
            case IDLE: {
                max_cones = msg.cones.size();
                state = ACCELERATE;
                vehicle_status.throttle = 1;
                vehicle_status.brake = 0;
                break;
            }
            case ACCELERATE: {
                if (msg.cones.size() - 1 < (max_cones / 2)) {
                    state = BRAKE;
                    vehicle_status.throttle = 0;
                    vehicle_status.brake = 1;
                }
                break;
            }
            case BRAKE: {
                if (vehicle_status.vehicle_speed == 0) {
                    state = FINISH;
                }
                break;
            }
            case FINISH: {
                RCLCPP_INFO(this->get_logger(), "END");
                state = END;
                break;
            }
            case END: {
                break;
            }
        }
        update_AS_CMD();
    }

    void send_data() {
        geometry_msgs::msg::Vector3 measurements;
        measurements.x = output;
        measurements.y = vehicle_status.vehicle_speed;
        measurements.z = target_speed;

        plot_data_publisher_->publish(measurements);
    }

    void start_brake() {
        t_start = this->now().seconds();
        throttle_off_timer_->cancel();
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Braking");
        measure_speed = vehicle_status.vehicle_speed;
        vehicle_status.throttle = false;
        vehicle_status.brake = true;
        state = BRAKE;
    }

    void check() {
        if (!sim_ready) {
            return;
        }
        if (state == IDLE) {
            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Started Test");
            t_start = this->now().seconds();
            vehicle_status.throttle = 1;
            state = ACCELERATE;
        }

        if (state == ACCELERATE && (this->now().seconds() - t_start) >= 3.0) {
            state = BRAKE;
            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Braking");
            measure_speed = vehicle_status.vehicle_speed;
            vehicle_status.throttle = 0;
            vehicle_status.brake = 1;
            t_start = this->now().seconds();
        }

        if (state == BRAKE && vehicle_status.vehicle_speed == 0) {
            check_timer_->cancel();
            get_vehicle_stats((this->now().seconds() - t_start));
        }
    }

    void get_vehicle_stats(double t_brake) {
        double throttle_a, brake_a;
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Acceleration time: 3 s");
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Measured speed: %.3f m/s", measure_speed);
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Deceleration time: %.3f s", t_brake);

        throttle_a = measure_speed / 3.0;
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Throttle acceleration: %.3f m/s^2", throttle_a);

        brake_a = measure_speed / t_brake;
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Brake deceleration: (-) %.3f m/s^2", brake_a);

        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Finished Test");
    }

    void update_AS_CMD() {
        switch (state) {
            case IDLE: {
                vehicle_status.throttle = 0;
                vehicle_status.brake = 0;
                vehicle_status.mission_finished = false;
                break;
            }
            case ACCELERATE: {
                //vehicle_status.throttle = 1;
                //vehicle_status.brake = 0;
                vehicle_status.mission_finished = false;
                break;
            }
            case BRAKE: {
                //vehicle_status.throttle = 0;
                //vehicle_status.brake = 1;
                vehicle_status.mission_finished = false;
                break;
            }
            case FINISH: {
                vehicle_status.throttle = 0;
                vehicle_status.brake = 0;
                vehicle_status.mission_finished = true;
                break;
            }
            case END: {
                break;
            }
        }
    }

    // (Following this guide: https://italiancoders.it/guardians-of-the-can-bus-come-usare-il-can-bus-in-c-e-c-pt-1/)
    int initCAN() {
        // Open CAN Socket
        sckt = socket(PF_CAN, SOCK_RAW, CAN_RAW);
        if (sckt == -1) {
            RCLCPP_ERROR(this->get_logger(), "Can't open socket");
            return EXIT_FAILURE;
        } else {
            RCLCPP_INFO(this->get_logger(), "Socket open");
        }

        // set I/O attributes
        struct ifreq ifr;
        memset(&ifr, 0, sizeof(ifr));
        snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "%s", "vcan0");
        if (ioctl(sckt, SIOCGIFINDEX, &ifr) < 0) {
            RCLCPP_ERROR(this->get_logger(), "ioctl error");
            close(sckt);
            return EXIT_FAILURE;
        }

        // set address attributes
        //struct sockaddr_can addr;
        memset(&addr, 0, sizeof(addr));
        addr.can_ifindex = ifr.ifr_ifindex;
        addr.can_family = AF_CAN;

        // bind address to socket
        if (bind(sckt, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
            RCLCPP_ERROR(this->get_logger(), "bind error");
            close(sckt);
            return EXIT_FAILURE;
        }

        /*
        // to receive a frame:
        struct can_frame frame;
        if (read(socket, &frame, sizeof(struct can_frame)) == -1) {
            RCLCPP_ERROR(this->get_logger(), "read error");
            close(socket);
            return EXIT_FAILURE;
        }

        // Where read is:   ssize_t read(int fildes, void *buf, size_t nbyte);
        // While write is:  ssize_t write(int fildes, const void *buf, size_t nbyte);
        */
        return 0;
    }

    int send_AS_CMD() {      // takes: throttle, brake, mission_finished requests
        // First: create, initialize, and fill message content
        eagle_task_as_cmd_t cmd;
        eagle_task_as_cmd_init(&cmd);

        const double throttle_physical = vehicle_status.throttle;
        const double brake_physical = vehicle_status.brake;
        cmd.mission_finished = vehicle_status.mission_finished;

        // Second: check physical values
        if (!eagle_task_as_cmd_throttle_req_is_in_phys_range(throttle_physical)) {
            RCLCPP_ERROR(this->get_logger(), "throttle request not in physical range");
            return -1;
        }
        if (!eagle_task_as_cmd_brake_req_is_in_phys_range(brake_physical)) {
            RCLCPP_ERROR(this->get_logger(), "brake request not in physical range");
            return -1;
        }
        if (!eagle_task_as_cmd_mission_finished_is_in_phys_range(cmd.mission_finished)) {
            RCLCPP_ERROR(this->get_logger(), "mission_finished request not in physical range");
            return -1;
        }

        // Third: encode requests and pack message
        cmd.throttle_req = eagle_task_as_cmd_throttle_req_encode(throttle_physical);
        cmd.brake_req = eagle_task_as_cmd_brake_req_encode(brake_physical);
        cmd.mission_finished = eagle_task_as_cmd_mission_finished_encode(cmd.mission_finished);

            // Frame composition
        struct can_frame frame;
        memset(&frame, 0, sizeof(frame));
        frame.can_id = 0x100;       // AS_CMD -> 0x100
        frame.can_dlc = 8;
        eagle_task_as_cmd_pack(frame.data, &cmd, sizeof(can_frame));

        // Send message (frame) over CAN
        if (write(sckt, &frame, sizeof(struct can_frame)) == -1) {
            RCLCPP_ERROR(this->get_logger(), "write error");
            //close(sckt);
            return EXIT_FAILURE;
        }

        /*
         else {      // Debug print
            RCLCPP_INFO(this->get_logger(), "sent AS_CMD: %d, %d, %d", t, b, m);
        }
        */

        return 0;
    }

    double readSpeed() {
        /*
        * @param[out] dst_p Object to unpack the message into.
        * @param[in] src_p Message to unpack.
        * @param[in] size Size of src_p.
        int eagle_task_as_cmd_unpack(struct eagle_task_as_cmd_t *dst_p, const uint8_t *src_p, size_t size);
         */

        // Receive frame:
        struct can_frame frame;
        memset(&frame, 0, sizeof(frame));
        if (read(sckt, &frame, sizeof(struct can_frame)) == -1) {
            RCLCPP_ERROR(this->get_logger(), "read error");
            close(sckt);
            return EXIT_FAILURE;
        }

        if (frame.can_id != 0x200) {        // VEH_SPEED -> 0x200
            RCLCPP_WARN(this->get_logger(), "received from unexpected can_id");
            return -1;
        }

        // Unpack frame
        eagle_task_veh_speed_t raw_speed;
        if (eagle_task_veh_speed_unpack(&raw_speed, frame.data, sizeof(can_frame))) {
            RCLCPP_ERROR(this->get_logger(), "Speed unpack error");
            return -1;
        }
        sim_ready = true;
        vehicle_status.vehicle_speed = eagle_task_veh_speed_vehicle_speed_decode(raw_speed.vehicle_speed);

        return vehicle_status.vehicle_speed;
    }

    void print_speed() {
        //auto message = std_msgs::msg::Double();
        //message.data = readSpeed();
        //RCLCPP_INFO(this->get_logger(), "'%s'", message.data.c_str());
        //publisher_->publish(message);
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Vehicle Speed: %f m/s", readSpeed());
    }

    private:
    fsmState state = IDLE;
    int max_cones = 0;
    double t_start = 0;
    double measure_speed = 0;
    bool sim_ready = false;

    int sckt;
    struct sockaddr_can addr;
    float RANGE_SIGHT;       // in meters
    double THROTTLE_A;       // m/s^2
    double BRAKE_A;         // m/s^2 (module)
    float KP;
    float KI;
    float KD;
    float target_speed;
    float output = 0;
    float error_sum = 0;
    double prev_error = 0;
    double prev_time = 0;
    bool first_iteration = true;

        // Declaration of timer, publisher and counter fields
        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
        rclcpp::Publisher<geometry_msgs::msg::Vector3>::SharedPtr plot_data_publisher_;
        size_t count_;

        rclcpp::TimerBase::SharedPtr speed_timer_;
        rclcpp::TimerBase::SharedPtr throttle_off_timer_;
        rclcpp::TimerBase::SharedPtr check_timer_;
        rclcpp::TimerBase::SharedPtr publish_data_timer_;

        rclcpp::TimerBase::SharedPtr send_timer_;
        rclcpp::Subscription<eagle_msgs::msg::ConeArray>::SharedPtr subscription_;
};

int main(int argc, char **argv) {
	rclcpp::init(argc, argv);
	RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Starting ctrl Node");

	rclcpp::spin(std::make_shared<Publisher>());

    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Shutting down");
	rclcpp::shutdown();
	return 0;
}
