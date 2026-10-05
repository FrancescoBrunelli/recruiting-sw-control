// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from eagle_msgs:msg/Cone.idl
// generated code does not contain a copyright notice

#ifndef EAGLE_MSGS__MSG__DETAIL__CONE__BUILDER_HPP_
#define EAGLE_MSGS__MSG__DETAIL__CONE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "eagle_msgs/msg/detail/cone__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace eagle_msgs
{

namespace msg
{

namespace builder
{

class Init_Cone_color
{
public:
  explicit Init_Cone_color(::eagle_msgs::msg::Cone & msg)
  : msg_(msg)
  {}
  ::eagle_msgs::msg::Cone color(::eagle_msgs::msg::Cone::_color_type arg)
  {
    msg_.color = std::move(arg);
    return std::move(msg_);
  }

private:
  ::eagle_msgs::msg::Cone msg_;
};

class Init_Cone_position
{
public:
  Init_Cone_position()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Cone_color position(::eagle_msgs::msg::Cone::_position_type arg)
  {
    msg_.position = std::move(arg);
    return Init_Cone_color(msg_);
  }

private:
  ::eagle_msgs::msg::Cone msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::eagle_msgs::msg::Cone>()
{
  return eagle_msgs::msg::builder::Init_Cone_position();
}

}  // namespace eagle_msgs

#endif  // EAGLE_MSGS__MSG__DETAIL__CONE__BUILDER_HPP_
