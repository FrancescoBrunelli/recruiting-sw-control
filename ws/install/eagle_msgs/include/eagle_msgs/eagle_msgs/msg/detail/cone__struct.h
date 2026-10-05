// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from eagle_msgs:msg/Cone.idl
// generated code does not contain a copyright notice

#ifndef EAGLE_MSGS__MSG__DETAIL__CONE__STRUCT_H_
#define EAGLE_MSGS__MSG__DETAIL__CONE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Constant 'BLUE'.
enum
{
  eagle_msgs__msg__Cone__BLUE = 0
};

/// Constant 'YELLOW'.
enum
{
  eagle_msgs__msg__Cone__YELLOW = 1
};

/// Constant 'ORANGE_SMALL'.
enum
{
  eagle_msgs__msg__Cone__ORANGE_SMALL = 2
};

/// Constant 'ORANGE_BIG'.
enum
{
  eagle_msgs__msg__Cone__ORANGE_BIG = 3
};

/// Constant 'UNKNOWN'.
enum
{
  eagle_msgs__msg__Cone__UNKNOWN = 4
};

// Include directives for member types
// Member 'position'
#include "geometry_msgs/msg/detail/point__struct.h"

/// Struct defined in msg/Cone in the package eagle_msgs.
typedef struct eagle_msgs__msg__Cone
{
  geometry_msgs__msg__Point position;
  uint8_t color;
} eagle_msgs__msg__Cone;

// Struct for a sequence of eagle_msgs__msg__Cone.
typedef struct eagle_msgs__msg__Cone__Sequence
{
  eagle_msgs__msg__Cone * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} eagle_msgs__msg__Cone__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // EAGLE_MSGS__MSG__DETAIL__CONE__STRUCT_H_
