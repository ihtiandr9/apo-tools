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
include(${BUILD_DIR}/rules/bkasm.cmake)

