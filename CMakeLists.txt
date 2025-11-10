cmake_minimum_required(VERSION 3.20)

project(ttfs CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(ttfs INTERFACE)

target_include_directories(ttfs INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set(SANDBOX_DIR ${CMAKE_SOURCE_DIR}/sandbox)
set(SANDBOX_CMAKELISTS ${SANDBOX_DIR}/CMakeLists.txt)

option(TTFS_BUILD_SANDBOX "Build the ttfs sandbox program" ON)

if(TTFS_BUILD_SANDBOX AND EXISTS ${SANDBOX_DIR} AND EXISTS ${SANDBOX_CMAKELISTS})
  add_subdirectory(${SANDBOX_DIR})
endif()
