cmake_minimum_required(VERSION 3.0...3.30)

    set(CMAKE_C_COMPILER gcc)
    set(CMAKE_CXX_COMPILER g++)
    set(CMAKE_ASM_COMPILER gcc)

    get_filename_component(PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)
    set(BUILD_DIR "${PROJECT_DIR}/Build")

    if(WIN32)
        include(${BUILD_DIR}/rules/tools_btstrp.cmake)
    endif()
project(bkasm C)

# glibc hides POSIX functions (localtime_r, fileno) from <time.h>/<stdio.h>
# in strict ANSI mode (-std=c11), which breaks clog.h error logging.
if(NOT WIN32)
    add_definitions(-D_POSIX_C_SOURCE=200809L)
endif()

include(${BUILD_DIR}/rules/bkasm.cmake)

