#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "eagle_msgs/msg/cone_array.hpp"
#include "eagle_msgs/msg/cone.hpp"

using std::placeholders::_1;

int cones_seen = 0;


class Subscriber : public rclcpp::Node
{
    public:
        Subscriber()
        : Node("centerline")
        {
          subscription_ = this->create_subscription<eagle_msgs::msg::ConeArray>(        // (need to convert each / into ::)
          "perception/cones", 10, std::bind(&Subscriber::topic_callback, this, _1));                    // Subscribe to /perception/cones
          publisher_ = this->create_publisher<std_msgs::msg::String>("planning/centerline", 10);        // Publish to /planning/centerline
        }

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
        void topic_callback(const eagle_msgs::msg::ConeArray & msg) const
        {
            /*
            // msg contains an array of cones: Cones[]
            RCLCPP_INFO(this->get_logger(), "msg: %d", (int) msg.cones.size());     // Print number of elements in the cones array
            RCLCPP_INFO(this->get_logger(), "color: %d", msg.cones[1].color);
            RCLCPP_INFO(this->get_logger(), "First cone position: (x: %f; y: %f; z: %f)", msg.cones[0].position.x, msg.cones[0].position.y, msg.cones[0].position.z);     // Print coordinates of first cone in the array
            */

        }
        rclcpp::Subscription<eagle_msgs::msg::ConeArray>::SharedPtr subscription_;
        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;

};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
	  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Starting ctrl Node");

    rclcpp::spin(std::make_shared<Subscriber>());

    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Shutting down");
	  rclcpp::shutdown();
    return 0;
}
