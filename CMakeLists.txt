cmake_minimum_required(VERSION 3.20)

project(ttfs CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_library(ttfs STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ttfs.cc
)
