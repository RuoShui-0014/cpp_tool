cmake_minimum_required(VERSION 3.31)
project(test)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(../base)

add_executable(test_base
    main.cpp
)