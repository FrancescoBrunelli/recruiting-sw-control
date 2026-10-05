// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from eagle_msgs:msg/ConeArray.idl
// generated code does not contain a copyright notice

#ifndef EAGLE_MSGS__MSG__DETAIL__CONE_ARRAY__STRUCT_H_
#define EAGLE_MSGS__MSG__DETAIL__CONE_ARRAY__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.h"
// Member 'cones'
#include "eagle_msgs/msg/detail/cone__struct.h"

/// Struct defined in msg/ConeArray in the package eagle_msgs.
/**
  * Cones sorted by ascending distance from the vehicle.
  * Frame is given in header.frame_id (base_link by default).
 */
typedef struct eagle_msgs__msg__ConeArray
{
  std_msgs__msg__Header header;
  eagle_msgs__msg__Cone__Sequence cones;
} eagle_msgs__msg__ConeArray;

// Struct for a sequence of eagle_msgs__msg__ConeArray.
typedef struct eagle_msgs__msg__ConeArray__Sequence
{
  eagle_msgs__msg__ConeArray * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} eagle_msgs__msg__ConeArray__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // EAGLE_MSGS__MSG__DETAIL__CONE_ARRAY__STRUCT_H_
