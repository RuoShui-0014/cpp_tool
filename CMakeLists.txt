cmake_minimum_required(VERSION 3.31)
project(cpp_tool)

set(CMAKE_CXX_STANDARD 20)

set(BUILDING_SHARED OFF)

add_subdirectory(base)
add_subdirectory(test)
