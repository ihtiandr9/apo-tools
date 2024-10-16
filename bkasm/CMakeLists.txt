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

## forktest
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
src/generator.c
)

##target_link_libraries(nonestl curl json-c)

add_custom_target(debug
COMMAND	gdb ./bkasm
COMMENT "GDB Debug"
DEPENDS ./bkasm
)

add_custom_target(unittest
COMMAND	./bkasm ../src/tests/test.asm
COMMENT "Run Tests"
DEPENDS ./bkasm
)

add_custom_target(testscript
COMMAND	python3 ../src/tests.py
COMMENT "Run Tests"
DEPENDS ./bkasm
)

add_custom_target(valgrind
COMMAND	valgrind -s ./bkasm ../src/tests/test.asm
COMMENT "Run Memory tests"
DEPENDS ./bkasm
)

install(TARGETS bkasm DESTINATION /usr/bin)
