cmake_minimum_required(VERSION 3.31)

project(base CXX)

set(CMAKE_CXX_STANDARD 20)

add_definitions(-DOS_WIN)

if (BUILDING_SHARED)
    add_definitions(-DBUILDING_SHARED)
    add_library(base SHARED
            macro.h
            thread_pool.cpp
            thread_pool.h
    )

    add_custom_command(TARGET base POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:base>
            $<TARGET_FILE_DIR:test>
            COMMENT "Copying DLL to executable directory"
    )
else ()
    add_library(base STATIC
        macro.h
        thread_pool.cpp
        thread_pool.h
    )
endif ()

