cmake_minimum_required(VERSION 3.5)
project(bkasm)
if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories(/usr/local/include
    /usr/include
    src)
link_directories(/usr/local/lib)

if(NOT DEFINED _Release_)
    add_definitions(-ggdb -Wno-switch)
endif()

## main executable file
add_executable(bkasm
    src/main.c
    src/globals.c
    src/errors.c
    src/lexer.c
    src/parser.c
    src/symbols.c
    src/inbuf.c
    src/cfg_tree.c
    src/nodes.c
    src/mathexpr.c
    src/bkints.c
    src/generator.c
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
    COMMAND	valgrind -s ./bkasm ../tests/test.asm
    COMMENT "Run Memory tests"
    DEPENDS ./bkasm
)

install(TARGETS bkasm DESTINATION /usr/bin)
