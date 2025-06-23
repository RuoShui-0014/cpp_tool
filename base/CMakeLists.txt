cmake_minimum_required(VERSION 3.31)

project(base CXX)

set(CMAKE_CXX_STANDARD 20)

#add_definitions(-DOS_WIN)
#add_definitions(-DBUILDING_SHARED)

add_library(base STATIC
    macro.h
    thread_pool.cpp
    thread_pool.h
)

target_include_directories(base PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})