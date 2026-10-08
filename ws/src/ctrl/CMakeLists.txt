cmake_minimum_required(VERSION 3.8)
project(ctrl)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(eagle_msgs REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

# Add libraries
add_library(ctrl_can STATIC can/eagle_task.c)

# Add executables
add_executable(ctrl_publisher src/ctrl.cpp)
add_executable(ctrl_subscriber src/ctrl_subscriber.cpp)

# name visibility folder
target_include_directories(ctrl_can PUBLIC can)

# Link library (ctrl_can) to ctrl package (ctrl_publisher)
target_link_libraries(ctrl_publisher ctrl_can)

# https://docs.ros.org/en/humble/Tutorials/Beginner-Client-Libraries/Writing-A-Simple-Cpp-Service-And-Client.html

# Need to link the dependencies
ament_target_dependencies(ctrl_publisher rclcpp std_msgs eagle_msgs)
ament_target_dependencies(ctrl_subscriber rclcpp std_msgs eagle_msgs)

# Need to install the target
# NOTE: share/ for launch files; lib/ for executables
install(TARGETS
    ctrl_publisher
    ctrl_subscriber

    DESTINATION lib/${PROJECT_NAME})

# Install launch file(s)
install(DIRECTORY
    launch

    DESTINATION share/${PROJECT_NAME})

ament_package()
