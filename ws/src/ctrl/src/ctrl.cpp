#include "ctrl.h"
#include "rclcpp/rclcpp.hpp"

int main(int argc, char **argv) {
	rclcpp::init(argc, argv);
	RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Hello World!");

	rclcpp::shutdown();
	return 0;
}
