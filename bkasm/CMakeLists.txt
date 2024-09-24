cmake_minimum_required(VERSION 3.5)
project(bkasm)
if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories(/usr/local/include
 /usr/include
 src)
link_directories(/usr/local/lib)

add_definitions(-g -Wno-switch)

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
)

##target_link_libraries(nonestl curl json-c)

add_custom_target(tests
COMMAND	./bkasm ../src/test.asm
COMMENT "Run Tests"
DEPENDS ./bkasm
)

install(TARGETS bkasm DESTINATION /usr/bin)
