// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from eagle_msgs:msg/ConeArray.idl
// generated code does not contain a copyright notice
#include "eagle_msgs/msg/detail/cone_array__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"
// Member `cones`
#include "eagle_msgs/msg/detail/cone__functions.h"

bool
eagle_msgs__msg__ConeArray__init(eagle_msgs__msg__ConeArray * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    eagle_msgs__msg__ConeArray__fini(msg);
    return false;
  }
  // cones
  if (!eagle_msgs__msg__Cone__Sequence__init(&msg->cones, 0)) {
    eagle_msgs__msg__ConeArray__fini(msg);
    return false;
  }
  return true;
}

void
eagle_msgs__msg__ConeArray__fini(eagle_msgs__msg__ConeArray * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // cones
  eagle_msgs__msg__Cone__Sequence__fini(&msg->cones);
}

bool
eagle_msgs__msg__ConeArray__are_equal(const eagle_msgs__msg__ConeArray * lhs, const eagle_msgs__msg__ConeArray * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__are_equal(
      &(lhs->header), &(rhs->header)))
  {
    return false;
  }
  // cones
  if (!eagle_msgs__msg__Cone__Sequence__are_equal(
      &(lhs->cones), &(rhs->cones)))
  {
    return false;
  }
  return true;
}

bool
eagle_msgs__msg__ConeArray__copy(
  const eagle_msgs__msg__ConeArray * input,
  eagle_msgs__msg__ConeArray * output)
{
  if (!input || !output) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__copy(
      &(input->header), &(output->header)))
  {
    return false;
  }
  // cones
  if (!eagle_msgs__msg__Cone__Sequence__copy(
      &(input->cones), &(output->cones)))
  {
    return false;
  }
  return true;
}

eagle_msgs__msg__ConeArray *
eagle_msgs__msg__ConeArray__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  eagle_msgs__msg__ConeArray * msg = (eagle_msgs__msg__ConeArray *)allocator.allocate(sizeof(eagle_msgs__msg__ConeArray), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(eagle_msgs__msg__ConeArray));
  bool success = eagle_msgs__msg__ConeArray__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
eagle_msgs__msg__ConeArray__destroy(eagle_msgs__msg__ConeArray * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    eagle_msgs__msg__ConeArray__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
eagle_msgs__msg__ConeArray__Sequence__init(eagle_msgs__msg__ConeArray__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  eagle_msgs__msg__ConeArray * data = NULL;

  if (size) {
    data = (eagle_msgs__msg__ConeArray *)allocator.zero_allocate(size, sizeof(eagle_msgs__msg__ConeArray), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = eagle_msgs__msg__ConeArray__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        eagle_msgs__msg__ConeArray__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
eagle_msgs__msg__ConeArray__Sequence__fini(eagle_msgs__msg__ConeArray__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      eagle_msgs__msg__ConeArray__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

eagle_msgs__msg__ConeArray__Sequence *
eagle_msgs__msg__ConeArray__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  eagle_msgs__msg__ConeArray__Sequence * array = (eagle_msgs__msg__ConeArray__Sequence *)allocator.allocate(sizeof(eagle_msgs__msg__ConeArray__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = eagle_msgs__msg__ConeArray__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
eagle_msgs__msg__ConeArray__Sequence__destroy(eagle_msgs__msg__ConeArray__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    eagle_msgs__msg__ConeArray__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
eagle_msgs__msg__ConeArray__Sequence__are_equal(const eagle_msgs__msg__ConeArray__Sequence * lhs, const eagle_msgs__msg__ConeArray__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!eagle_msgs__msg__ConeArray__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
eagle_msgs__msg__ConeArray__Sequence__copy(
  const eagle_msgs__msg__ConeArray__Sequence * input,
  eagle_msgs__msg__ConeArray__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(eagle_msgs__msg__ConeArray);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    eagle_msgs__msg__ConeArray * data =
      (eagle_msgs__msg__ConeArray *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!eagle_msgs__msg__ConeArray__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          eagle_msgs__msg__ConeArray__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!eagle_msgs__msg__ConeArray__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
