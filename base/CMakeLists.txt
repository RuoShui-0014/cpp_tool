cmake_minimum_required(VERSION 3.31)

project(base CXX)

set(CMAKE_CXX_STANDARD 20)

add_definitions(-DOS_WIN -DBUILDING_SHARED)

add_library(base SHARED
    thread_pool.cpp
    thread_pool.h
    macro.h
)