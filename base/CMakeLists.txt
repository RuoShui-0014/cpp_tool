cmake_minimum_required(VERSION 3.31)

project(base CXX)

set(CMAKE_CXX_STANDARD 20)

if(WIN32)
    add_compile_definitions(OS_WIN)
endif()

set(BASE_SOURCES
        logger.cpp
        thread_pool.cpp
)

set(BASE_HEADERS
        macro.h
        logger.h
        thread_pool.h
        safe_ptr.h
)

if(BUILDING_SHARED)
    add_library(base SHARED ${BASE_SOURCES} ${BASE_HEADERS})
    target_compile_definitions(base PRIVATE BUILDING_SHARED)

    # 只有当 test 目标存在时才添加拷贝命令
    if(TARGET test)
        add_custom_command(TARGET base POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:base>
                $<TARGET_FILE_DIR:test>
                COMMENT "Copying DLL to test directory"
        )
    endif()
else()
    add_library(base STATIC ${BASE_SOURCES} ${BASE_HEADERS})
endif()
