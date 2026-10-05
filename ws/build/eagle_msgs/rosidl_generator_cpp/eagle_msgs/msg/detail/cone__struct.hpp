// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from eagle_msgs:msg/Cone.idl
// generated code does not contain a copyright notice

#ifndef EAGLE_MSGS__MSG__DETAIL__CONE__STRUCT_HPP_
#define EAGLE_MSGS__MSG__DETAIL__CONE__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'position'
#include "geometry_msgs/msg/detail/point__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__eagle_msgs__msg__Cone __attribute__((deprecated))
#else
# define DEPRECATED__eagle_msgs__msg__Cone __declspec(deprecated)
#endif

namespace eagle_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct Cone_
{
  using Type = Cone_<ContainerAllocator>;

  explicit Cone_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : position(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->color = 0;
    }
  }

  explicit Cone_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : position(_alloc, _init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->color = 0;
    }
  }

  // field types and members
  using _position_type =
    geometry_msgs::msg::Point_<ContainerAllocator>;
  _position_type position;
  using _color_type =
    uint8_t;
  _color_type color;

  // setters for named parameter idiom
  Type & set__position(
    const geometry_msgs::msg::Point_<ContainerAllocator> & _arg)
  {
    this->position = _arg;
    return *this;
  }
  Type & set__color(
    const uint8_t & _arg)
  {
    this->color = _arg;
    return *this;
  }

  // constant declarations
  static constexpr uint8_t BLUE =
    0u;
  static constexpr uint8_t YELLOW =
    1u;
  static constexpr uint8_t ORANGE_SMALL =
    2u;
  static constexpr uint8_t ORANGE_BIG =
    3u;
  static constexpr uint8_t UNKNOWN =
    4u;

  // pointer types
  using RawPtr =
    eagle_msgs::msg::Cone_<ContainerAllocator> *;
  using ConstRawPtr =
    const eagle_msgs::msg::Cone_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<eagle_msgs::msg::Cone_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<eagle_msgs::msg::Cone_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      eagle_msgs::msg::Cone_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<eagle_msgs::msg::Cone_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      eagle_msgs::msg::Cone_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<eagle_msgs::msg::Cone_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<eagle_msgs::msg::Cone_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<eagle_msgs::msg::Cone_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__eagle_msgs__msg__Cone
    std::shared_ptr<eagle_msgs::msg::Cone_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__eagle_msgs__msg__Cone
    std::shared_ptr<eagle_msgs::msg::Cone_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Cone_ & other) const
  {
    if (this->position != other.position) {
      return false;
    }
    if (this->color != other.color) {
      return false;
    }
    return true;
  }
  bool operator!=(const Cone_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Cone_

// alias to use template instance with default allocator
using Cone =
  eagle_msgs::msg::Cone_<std::allocator<void>>;

// constant definitions
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t Cone_<ContainerAllocator>::BLUE;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t Cone_<ContainerAllocator>::YELLOW;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t Cone_<ContainerAllocator>::ORANGE_SMALL;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t Cone_<ContainerAllocator>::ORANGE_BIG;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t Cone_<ContainerAllocator>::UNKNOWN;
#endif  // __cplusplus < 201703L

}  // namespace msg

}  // namespace eagle_msgs

#endif  // EAGLE_MSGS__MSG__DETAIL__CONE__STRUCT_HPP_
