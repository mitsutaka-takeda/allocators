project (allocators)

cmake_minimum_required(VERSION 3.2)

set(GUIDELINES_SUPPORT_LIBRARY_INCLUDE_DIRECTORY /home/mtakeda/libs/GSL/include)

add_library(allocators
  affix_allocator.cpp
  affix_allocator.h
  block.cpp
  block.h
  detail/unique_resource.h
  fallback_allocator.cpp
  fallback_allocator.h
  free_list_allocator.cpp
  free_list_allocator.h
  null_allocator.cpp
  null_allocator.h
  stack_allocator.cpp
  stack_allocator.h
  )

target_include_directories(
  allocators
  SYSTEM
  PRIVATE
  ${GUIDELINES_SUPPORT_LIBRARY_INCLUDE_DIRECTORY}
)

target_compile_options(
  allocators
  PRIVATE
  -std=c++14
  -Werror
  -Weverything
  -Wno-c++98-compat
)
