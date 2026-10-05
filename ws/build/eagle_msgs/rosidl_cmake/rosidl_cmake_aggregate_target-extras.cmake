# generated from rosidl_cmake/cmake/rosidl_cmake_aggregate_target-extras.cmake.in

# Create a convenience aggregate target eagle_msgs::eagle_msgs
# that links all generated interface targets, so downstream packages can use
# a single modern CMake target name instead of ${eagle_msgs_TARGETS}.
if(eagle_msgs_TARGETS AND NOT TARGET eagle_msgs::eagle_msgs)
  add_library(eagle_msgs::eagle_msgs INTERFACE IMPORTED)
  set_target_properties(eagle_msgs::eagle_msgs PROPERTIES
    INTERFACE_LINK_LIBRARIES "${eagle_msgs_TARGETS}")
endif()
