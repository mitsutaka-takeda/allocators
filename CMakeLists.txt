project (allocators)

cmake_minimum_required(VERSION 3.2)

set(GUIDELINES_SUPPORT_LIBRARY_INCLUDE_DIRECTORY /home/mtakeda/libs/GSL/include)

add_library(allocators
  block.h
  block.cpp
  )
