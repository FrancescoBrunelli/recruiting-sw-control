#include "ctrl.h"
#include <chrono>
#include <string>
#include <memory>
#include <functional>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class MinimalPublisher : public rclcpp::Node {
    public:
        MinimalPublisher() : Node("minimal_publisher"), count_(0) {     // Node naming and init count to 0
            publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);     // Publisher initialized with stirng type and topic named "topic"
            timer_ = this->create_wall_timer(500ms, std::bind(&MinimalPublisher::timer_callback, this));    // init timer, timer_callback gets executed twice a second
        }

    private:
        void timer_callback(){
            auto message = std_msgs::msg::String();
            message.data = "Hello World!" + std::to_string(count_++);
            RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());  // Every published message gets printed to console
            publisher_->publish(message);
        }
        // Declaration of timer, publisher and counter fields
        rclcpp::TimerBase::SharedPtr timer_;
        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
        size_t count_;
};


int main(int argc, char **argv) {
	rclcpp::init(argc, argv);
	RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Starting ctrl Node");

	rclcpp::spin(std::make_shared<MinimalPublisher>());

    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Shutting down");
	rclcpp::shutdown();
	return 0;
}
