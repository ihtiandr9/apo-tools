
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

## unittest tests one unit

add_custom_target(unittest
    COMMAND	$<TARGET_FILE:bkasm> ${PROJECT_DIR}/tests/test.asm
    COMMENT "Run Test"
    DEPENDS bkasm
)

## script unittests

add_custom_target(run_tests
    COMMAND	../tests/python/bin/python3 ../tests/tests.py
    COMMENT "Run Tests"
    DEPENDS ./bkasm
)

## valgrind test

add_custom_target(valgrind
##    COMMAND	valgrind -s --leak-check=full ./bkasm ../tests/test.asm
    COMMAND	valgrind -s ./bkasm ../tests/test.asm
    COMMENT "Run Memory tests"
    DEPENDS ./bkasm
)

install(TARGETS bkasm DESTINATION /usr/local/bin)
