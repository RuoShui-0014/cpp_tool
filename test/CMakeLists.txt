cmake_minimum_required(VERSION 3.31)
project(test)

set(CMAKE_CXX_STANDARD 20)

include_directories(../)
add_definitions(-DOS_WIN)

if (BUILDING_SHARED)
add_definitions(-DUSING_SHARED)
endif ()

add_executable(test
    main.cpp
)

target_link_libraries(test PRIVATE base)
