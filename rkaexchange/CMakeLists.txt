cmake_minimum_required(VERSION 3.0)
project(rkaexchange)
if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories(/usr/local/include
 /usr/include
 src)
link_directories(/usr/local/lib)

## forktest
add_executable(rkaexchange
src/main.c
src/edb.c
src/errors.c
src/inbuf.c
)

##target_link_libraries(nonestl curl json-c)
add_definitions(-g)

install(TARGETS rkaexchange  DESTINATION bin)
