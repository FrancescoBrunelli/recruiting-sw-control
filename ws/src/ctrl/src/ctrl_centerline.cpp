#include <memory>
#include <vector>
#include <chrono>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "eagle_msgs/msg/cone_array.hpp"
#include "eagle_msgs/msg/cone.hpp"

#include "geometry_msgs/msg/pose.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "nav_msgs/msg/path.hpp"
#include "visualization_msgs/msg/marker.hpp"

using std::placeholders::_1;

//using namespace std;
using namespace std::chrono_literals;

class Subscriber : public rclcpp::Node
{
    public:
        Subscriber()
        : Node("centerline")
        {
            subscription_ = this->create_subscription<eagle_msgs::msg::ConeArray>(        // (need to convert each / into ::)
              "perception/cones", 10, std::bind(&Subscriber::updateCones, this, _1));                    // Subscribe to /perception/cones
            publisher_ = this->create_publisher<nav_msgs::msg::Path>("planning/centerline", 10);        // Publish to /planning/centerline
            marker_publisher_ = this->create_publisher<visualization_msgs::msg::Marker>("centerline/marker", 10);       // Path line marker publisher
            update_path_timer_ = this->create_wall_timer(100ms, std::bind(&Subscriber::publishPath, this));             // Update marker every 100ms

            points.ns = markerLine.ns = "points and lines";
            points.action = markerLine.action = visualization_msgs::msg::Marker::ADD;
            points.id = 0;
            markerLine.id = 1;
            points.type = visualization_msgs::msg::Marker::POINTS;
            markerLine.type = visualization_msgs::msg::Marker::LINE_STRIP;
            points.scale.x = 0.02;
            points.scale.y = 0.02;
            markerLine.scale.x = 0.01;
            markerLine.scale.y = 0.01;
            points.color.g = 1.0f;
            points.color.a = 1.0;
            markerLine.color.b = 1.0f;
            markerLine.color.a = 1.0;
        }

        // (https://docs.ros.org/en/humble/Tutorials/Intermediate/RViz/Marker-Points-and-Lines/Marker-Points-and-Lines.html)


    /*
eagle@francesco-macbookair:~/ws$ ros2 interface show eagle_msgs/msg/ConeArray
# Cones sorted by ascending distance from the vehicle.
# Frame is given in header.frame_id (base_link by default).
std_msgs/Header header
    builtin_interfaces/Time stamp
            int32 sec
            uint32 nanosec
    string frame_id
Cone[] cones
    uint8 BLUE=0
    uint8 YELLOW=1
    uint8 ORANGE_SMALL=2
    uint8 ORANGE_BIG=3
    uint8 UNKNOWN=4
    geometry_msgs/Point position
            float64 x
            float64 y
            float64 z
    uint8 color
eagle@francesco-macbookair:~/ws$ ros2 interface show eagle_msgs/msg/Cone
uint8 BLUE=0
uint8 YELLOW=1
uint8 ORANGE_SMALL=2
uint8 ORANGE_BIG=3
uint8 UNKNOWN=4

geometry_msgs/Point position
    float64 x
    float64 y
    float64 z
uint8 color

eagle@francesco-macbookair:~/ws$ ros2 topic echo /perception/cones
header:
stamp:
sec: 1791624621
nanosec: 248072519
frame_id: base_link
cones:
- position:
x: 5.0
y: 2.0
z: 0.0
color: 0
- position:
x: 5.0
y: -2.0
z: 0.0
color: 1
...
    */

    private:
        void updateCones(const eagle_msgs::msg::ConeArray & msg)
        {
            points.points.clear();      // Ensure old points are cleared
            markerLine.points.clear();  // Ensure old points are cleared
            path.poses.clear();
            /*
            // msg contains an array of cones: Cones[]
            RCLCPP_INFO(this->get_logger(), "msg: %d", (int) msg.cones.size());     // Print number of elements in the cones array
            RCLCPP_INFO(this->get_logger(), "color: %d", msg.cones[1].color);
            RCLCPP_INFO(this->get_logger(), "First cone position: (x: %f; y: %f; z: %f)", msg.cones[0].position.x, msg.cones[0].position.y, msg.cones[0].position.z);     // Print coordinates of first cone in the array
            */

            geometry_msgs::msg::PoseStamped poseStamped;
            for (int i = 0; i < msg.cones.size(); i++) {
                // To create centerline path we need to create first the Pose, then the PoseStamped and then append it to Path
                geometry_msgs::msg::Pose pose;
                geometry_msgs::msg::Point p;
                pose.position.x = msg.cones[i].position.x;
                p.x = pose.position.x;
                pose.position.y = ((msg.cones[i].position.y) + (msg.cones[++i].position.y)) / 2;
                p.y = pose.position.y;
                pose.position.z = msg.cones[i].position.z;
                p.z = pose.position.z;
                pose.orientation.w = 1.0;       // Keep orientation unchanged

                //RCLCPP_INFO(this->get_logger(), "Position %d: (x: %f; y: %f; z: %f)", (i-1), pose.position.x, pose.position.y, pose.position.z);
                poseStamped.pose = pose;
                poseStamped.header.frame_id = msg.header.frame_id;
                poseStamped.header.stamp = this->now();     // Put as header stamp the moment of creation
                path.poses.push_back(poseStamped);
                points.points.push_back(p);
                markerLine.points.push_back(p);
            }
            path.header.frame_id = msg.header.frame_id;
            path.header.stamp = this->now();

            // Update Marked line
            points.header.frame_id = markerLine.header.frame_id = msg.header.frame_id;
            points.header.stamp = markerLine.header.stamp = this->now();

        }

        void publishPath() {
            // Publish updated Path and Markers only if not empty
            if (path.header.frame_id.empty()) {
                return;
            }
            publisher_->publish(path);
            //RCLCPP_INFO(this->get_logger(), "Path published");        // Debug print

            // Publish updated points/line
            marker_publisher_->publish(points);
            marker_publisher_->publish(markerLine);
            //RCLCPP_INFO(this->get_logger(), "Markers published");     // Debug print
        }

        nav_msgs::msg::Path path;
        visualization_msgs::msg::Marker points, markerLine;

        rclcpp::Subscription<eagle_msgs::msg::ConeArray>::SharedPtr subscription_;
        rclcpp::Publisher<nav_msgs::msg::Path>::SharedPtr publisher_;
        rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr marker_publisher_;
        rclcpp::TimerBase::SharedPtr update_path_timer_;
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Starting ctrl Node");

    rclcpp::spin(std::make_shared<Subscriber>());

    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Shutting down");
    rclcpp::shutdown();
    return 0;
}
