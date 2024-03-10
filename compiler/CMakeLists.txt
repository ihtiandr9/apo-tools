cmake_minimum_required(VERSION 3.5)
project(bkasm)
if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories(/usr/local/include
 /usr/include
 src)
link_directories(/usr/local/lib)

## forktest
add_executable(bkasm
src/main.c
src/globals.c
src/errors.c
src/lexer.c
src/parser.c
src/symbols.c
src/inbuf.c
src/program.c
src/nodes.c
)

##target_link_libraries(nonestl curl json-c)
add_definitions(-g)

add_custom_target(test
COMMAND	./bkasm ../src/test.asm
COMMENT "Run Tests"
)

install(TARGETS bkasm DESTINATION bin)
