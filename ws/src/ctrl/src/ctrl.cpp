#include "ctrl.h"
#include <chrono>                       // time utils
#include <string>
#include <memory>                       // smart pointers
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

using namespace std::chrono_literals;

VehicleStatus vehicle_status(false, false, false, 0);

int sckt;
struct sockaddr_can addr;

class Publisher : public rclcpp::Node {
public:
    Publisher() : Node("minimal_publisher"), count_(0) {     // Node naming and init count to 0
        publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);     // Publisher initialized with stirng type and topic named "topic"
        initCAN();
        //timer_ = this->create_wall_timer(500ms, std::bind(&MinimalPublisher::timer_callback, this));    // init timer, timer_callback gets executed twice a second
        //timer_ = this->create_wall_timer(5ms, std::bind(&Publisher::send_AS_CMD, this));
        speed_timer_ = this->create_wall_timer(1s, std::bind(&Publisher::print_speed, this));

        // To pass parameters: function, this, param1, param2, ...., param<n>
        //send_timer_ = this->create_wall_timer(5ms, std::bind(&Publisher::send_AS_CMD, this, false, false, false));
        send_timer_ = this->create_wall_timer(5ms, [this] () {send_AS_CMD(false, false, false);});
        //send_timer_ = this->create_wall_timer(5ms, [this] () {send_AS_CMD(true, false, false);});
    }

    // (Following this guide: https://italiancoders.it/guardians-of-the-can-bus-come-usare-il-can-bus-in-c-e-c-pt-1/)
    int initCAN() {
        // Open CAN Socket
        sckt = socket(PF_CAN, SOCK_RAW, CAN_RAW);
        if (sckt == -1) {
            RCLCPP_ERROR(this->get_logger(), "Can't open socket");
            return EXIT_FAILURE;
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

    int send_AS_CMD(bool t, bool b, bool m) {      // takes: throttle, brake, mission_finished requests
        // First: create, initialize, and fill message content
        eagle_task_as_cmd_t cmd;
        eagle_task_as_cmd_init(&cmd);
        cmd.throttle_req = t;
        cmd.brake_req = b;
        cmd.mission_finished = m;

        // Second: check physical values
        if (!eagle_task_as_cmd_throttle_req_is_in_phys_range(cmd.throttle_req)) {
            RCLCPP_ERROR(this->get_logger(), "throttle request not in physical range");
            return -1;
        }
        if (!eagle_task_as_cmd_brake_req_is_in_phys_range(cmd.brake_req)) {
            RCLCPP_ERROR(this->get_logger(), "brake request not in physical range");
            return -1;
        }
        if (!eagle_task_as_cmd_mission_finished_is_in_phys_range(cmd.mission_finished)) {
            RCLCPP_ERROR(this->get_logger(), "mission_finished request not in physical range");
            return -1;
        }

        // Third: encode requests and pack message
        cmd.throttle_req = eagle_task_as_cmd_throttle_req_encode(cmd.throttle_req);
        cmd.brake_req = eagle_task_as_cmd_brake_req_encode(cmd.brake_req);
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
        vehicle_status.vehicle_speed = eagle_task_veh_speed_vehicle_speed_decode(raw_speed.vehicle_speed);

        return vehicle_status.vehicle_speed;
    }



    private:
        void timer_callback(){
            auto message = std_msgs::msg::String();
            message.data = "Hello World!" + std::to_string(count_++);
            RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());  // Every published message gets printed to console
            publisher_->publish(message);
        }

    void print_speed() {
            //auto message = std_msgs::msg::Double();
            //message.data = readSpeed();
            //RCLCPP_INFO(this->get_logger(), "'%s'", message.data.c_str());
            //publisher_->publish(message);
            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Vehicle Speed: %f", readSpeed());
        }


        // Declaration of timer, publisher and counter fields
        rclcpp::TimerBase::SharedPtr timer_;
        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
        size_t count_;

        rclcpp::TimerBase::SharedPtr speed_timer_;
        rclcpp::TimerBase::SharedPtr send_timer_;
};



int main(int argc, char **argv) {
	rclcpp::init(argc, argv);
	RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Starting ctrl Node");

	rclcpp::spin(std::make_shared<Publisher>());

    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Shutting down");
	rclcpp::shutdown();
	return 0;
}

/*
//eagle_task_as_cmd_t cmd;
        eagle_task_veh_speed_t cmd;
        if (eagle_task_as_cmd_unpack(&cmd, frame.data, sizeof(struct can_frame)) != 0) {
            RCLCPP_ERROR(this->get_logger(), "Message unpack error");
            return 0;
        }

        // Unpack and Decode message contents
        eagle_task_veh_speed_t raw_speed;
        if (eagle_task_veh_speed_unpack(&raw_speed, cmd.vehicle_speed, sizeof(cmd))) {
            RCLCPP_ERROR(this->get_logger(), "Speed unpack error");
            return 0;
        }
        vehicle_status.vehicle_speed = eagle_task_veh_speed_vehicle_speed_decode(raw_speed.vehicle_speed);

        return vehicle_status.vehicle_speed;
 */
