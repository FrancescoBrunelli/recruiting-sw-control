#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
using std::placeholders::_1;

class Subscriber : public rclcpp::Node
{
    public:
        Subscriber()
        : Node("centerline")
        {
          subscription_ = this->create_subscription<std_msgs::msg::String>(
          "perception/cones", 10, std::bind(&Subscriber::topic_callback, this, _1));                    // Subscribe to /perception/cones
          publisher_ = this->create_publisher<std_msgs::msg::String>("planning/centerline", 10);        // Publish to /planning/centerline
        }

    private:
        void topic_callback(const std_msgs::msg::String & msg) const
        {
          RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg.data.c_str());
        }
        rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
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
