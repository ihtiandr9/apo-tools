cmake_minimum_required(VERSION 3.0...3.30)
project(bkasm)

include_directories(/usr/local/include
    /usr/include
    src)

link_directories(/usr/local/lib)

file(GLOB SOURCE_C src/*c)

if(NOT DEFINED _Release_)
    add_definitions(-ggdb -Wno-switch)
endif()

## main executable file
add_executable(bkasm
    ${SOURCE_C}
)

## debug target

add_custom_target(debug
    COMMAND	gdb ./bkasm
    COMMENT "GDB Debug"
    DEPENDS ./bkasm
)

## unittest tests one unit

add_custom_target(unittest
    COMMAND	./bkasm ../tests/test.asm
    COMMENT "Run Test"
    DEPENDS ./bkasm
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

install(TARGETS bkasm DESTINATION /usr/bin)
