
file(GLOB BKASM_SOURCE_C ${PROJECT_DIR}/src/*c)

if(NOT DEFINED _Release_)
    add_definitions(-ggdb -Wno-switch)
endif()

## main executable file
add_executable(bkasm
    ${BKASM_SOURCE_C}
)

target_include_directories(bkasm PRIVATE
    /usr/local/include
    /usr/include
    ${PROJECT_DIR}/src)

target_link_directories(bkasm PRIVATE
    /usr/local/lib)

target_compile_options(bkasm PRIVATE
 -std=c11 -Wall
)

set_target_properties(bkasm PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bkasm
)


## debug target

add_custom_target(debug
    COMMAND	gdb ${CMAKE_BINARY_DIR}/bkasm
    COMMENT "GDB Debug"
    DEPENDS ./bkasm
)

include(${BUILD_DIR}/rules/tests.cmake)

install(TARGETS bkasm DESTINATION /usr/local/bin)
