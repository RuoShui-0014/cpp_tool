cmake_minimum_required(VERSION 3.31)
project(test)

set(CMAKE_CXX_STANDARD 20)

#add_definitions(-DOS_WIN)
#add_definitions(-DUSING_SHARED)

add_executable(test
    main.cpp
)

target_link_libraries(test PRIVATE base)