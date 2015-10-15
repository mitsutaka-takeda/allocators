project (allocators)

cmake_minimum_required(VERSION 3.2)

set(GUIDELINES_SUPPORT_LIBRARY_INCLUDE_DIRECTORY /home/mtakeda/libs/GSL/include)

add_library(allocators
  block.h
  block.cpp
  null_allocator.h
  null_allocator.cpp
  fallback_allocator.h
  fallback_allocator.cpp
  stack_allocator.h
  stack_allocator.cpp
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
