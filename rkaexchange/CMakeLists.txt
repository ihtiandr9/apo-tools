cmake_minimum_required(VERSION 3.5)
project(rkaexchange)
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

# main executable target
add_executable(rkaexchange
    src/main.c
    src/edb.c
    src/errors.c
    src/inbuf.c
    src/globals.c
)

add_custom_target(run_tests
    COMMENT "Run Tests"
)

add_custom_target(testpack
    COMMAND ./rkaexchange pack ../src/test.asm
    COMMENT "Test pack"
    DEPENDS rkaexchange
)

add_custom_target(testunpack
    COMMAND ./rkaexchange unpack ../src/test.rka
    COMMENT "Test unpack"
    DEPENDS rkaexchange
)

add_custom_target(testinfileerror
    COMMAND ./rkaexchange unpack ../src/errtest.rka || (exit 0)
    COMMENT "Test infile error"
    DEPENDS rkaexchange
)

add_custom_target(testoutfileerror
    COMMAND chmod 000 ../src/errtest.asm
    COMMAND ./rkaexchange unpack ../src/test.rka ../src/errtest.asm || (exit 0)
    COMMAND chmod 666 ../src/errtest.asm
    COMMENT "Test outfile error"
    DEPENDS rkaexchange
)

add_dependencies(run_tests
    rkaexchange
    testpack
    testunpack
    testinfileerror
    testoutfileerror
)

## valgrind test

add_custom_target(valgrind
    COMMAND valgrind -s ./rkaexchange unpack ../src/test.rka
    COMMENT "Run Memory tests"
    DEPENDS ./rkaexchange
)

##  target_link_libraries(nonestl curl json-c)

install(TARGETS rkaexchange  DESTINATION bin)
