cmake_minimum_required(VERSION 3.8)

project(timemath)

add_executable(timemath timemath.c timemath.h)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wpedantic -Wall -std=c11")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic -Wall -std=c11")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Wall /std:c11")
endif()