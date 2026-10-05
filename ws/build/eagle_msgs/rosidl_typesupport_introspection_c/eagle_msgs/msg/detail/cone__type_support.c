// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from eagle_msgs:msg/Cone.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "eagle_msgs/msg/detail/cone__rosidl_typesupport_introspection_c.h"
#include "eagle_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "eagle_msgs/msg/detail/cone__functions.h"
#include "eagle_msgs/msg/detail/cone__struct.h"


// Include directives for member types
// Member `position`
#include "geometry_msgs/msg/point.h"
// Member `position`
#include "geometry_msgs/msg/detail/point__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void eagle_msgs__msg__Cone__rosidl_typesupport_introspection_c__Cone_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  eagle_msgs__msg__Cone__init(message_memory);
}

void eagle_msgs__msg__Cone__rosidl_typesupport_introspection_c__Cone_fini_function(void * message_memory)
{
  eagle_msgs__msg__Cone__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember eagle_msgs__msg__Cone__rosidl_typesupport_introspection_c__Cone_message_member_array[2] = {
  {
    "position",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(eagle_msgs__msg__Cone, position),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "color",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(eagle_msgs__msg__Cone, color),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers eagle_msgs__msg__Cone__rosidl_typesupport_introspection_c__Cone_message_members = {
  "eagle_msgs__msg",  // message namespace
  "Cone",  // message name
  2,  // number of fields
  sizeof(eagle_msgs__msg__Cone),
  eagle_msgs__msg__Cone__rosidl_typesupport_introspection_c__Cone_message_member_array,  // message members
  eagle_msgs__msg__Cone__rosidl_typesupport_introspection_c__Cone_init_function,  // function to initialize message memory (memory has to be allocated)
  eagle_msgs__msg__Cone__rosidl_typesupport_introspection_c__Cone_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t eagle_msgs__msg__Cone__rosidl_typesupport_introspection_c__Cone_message_type_support_handle = {
  0,
  &eagle_msgs__msg__Cone__rosidl_typesupport_introspection_c__Cone_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_eagle_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, eagle_msgs, msg, Cone)() {
  eagle_msgs__msg__Cone__rosidl_typesupport_introspection_c__Cone_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, geometry_msgs, msg, Point)();
  if (!eagle_msgs__msg__Cone__rosidl_typesupport_introspection_c__Cone_message_type_support_handle.typesupport_identifier) {
    eagle_msgs__msg__Cone__rosidl_typesupport_introspection_c__Cone_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &eagle_msgs__msg__Cone__rosidl_typesupport_introspection_c__Cone_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
