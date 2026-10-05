// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from eagle_msgs:msg/ConeArray.idl
// generated code does not contain a copyright notice

#ifndef EAGLE_MSGS__MSG__DETAIL__CONE_ARRAY__BUILDER_HPP_
#define EAGLE_MSGS__MSG__DETAIL__CONE_ARRAY__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "eagle_msgs/msg/detail/cone_array__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace eagle_msgs
{

namespace msg
{

namespace builder
{

class Init_ConeArray_cones
{
public:
  explicit Init_ConeArray_cones(::eagle_msgs::msg::ConeArray & msg)
  : msg_(msg)
  {}
  ::eagle_msgs::msg::ConeArray cones(::eagle_msgs::msg::ConeArray::_cones_type arg)
  {
    msg_.cones = std::move(arg);
    return std::move(msg_);
  }

private:
  ::eagle_msgs::msg::ConeArray msg_;
};

class Init_ConeArray_header
{
public:
  Init_ConeArray_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ConeArray_cones header(::eagle_msgs::msg::ConeArray::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_ConeArray_cones(msg_);
  }

private:
  ::eagle_msgs::msg::ConeArray msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::eagle_msgs::msg::ConeArray>()
{
  return eagle_msgs::msg::builder::Init_ConeArray_header();
}

}  // namespace eagle_msgs

#endif  // EAGLE_MSGS__MSG__DETAIL__CONE_ARRAY__BUILDER_HPP_
