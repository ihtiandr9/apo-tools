cmake_minimum_required(VERSION 3.0)
project(rkaexchange)
if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories(/usr/local/include
    /usr/include
    src)
link_directories(/usr/local/lib)

add_definitions(-g)

## forktest
add_executable(rkaexchange
    src/main.c
    src/edb.c
    src/errors.c
    src/inbuf.c
    src/globals.c
)

add_custom_target(test
    COMMENT "Run Tests"
)

add_custom_target(testpack
    COMMAND ./rkaexchange pack ../src/test.asm
    COMMENT "Test pack"
)

add_custom_target(testunpack
    COMMAND ./rkaexchange unpack ../src/test.rka
    COMMENT "Test unpack"
)

add_custom_target(testinfileerror
    COMMAND ./rkaexchange unpack ../src/errtest.rka || (exit 0)
    COMMENT "Test infile error"
)

add_custom_target(testoutfileerror
    COMMAND chmod 000 ../src/errtest.asm
    COMMAND ./rkaexchange unpack ../src/test.rka ../src/errtest.asm || (exit 0)
    COMMENT "Test outfile error"
)

add_dependencies(test
    rkaexchange
    testpack
    testunpack
    testinfileerror
    testoutfileerror
)

##target_link_libraries(nonestl curl json-c)

install(TARGETS rkaexchange  DESTINATION bin)
