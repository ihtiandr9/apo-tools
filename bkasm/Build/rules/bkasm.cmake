
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

## create python venv for tests if missing (not tracked in git — see root .gitignore)

add_custom_target(setup_venv
    COMMAND test -f ${PROJECT_DIR}/tests/python/bin/python3 || python3 -m venv --without-pip ${PROJECT_DIR}/tests/python
    COMMENT "Setup Python venv for tests"
)

## script unittests

add_custom_target(run_tests
    COMMAND ${CMAKE_COMMAND} -E env BKASM_BINARY=$<TARGET_FILE:bkasm> ${PROJECT_DIR}/tests/python/bin/python3 ${PROJECT_DIR}/tests/tests.py
    COMMAND ${CMAKE_COMMAND} -E env BKASM_BINARY=$<TARGET_FILE:bkasm> ${PROJECT_DIR}/tests/python/bin/python3 ${PROJECT_DIR}/tests/test_opcodes.py
    COMMAND ${CMAKE_COMMAND} -E env BKASM_BINARY=$<TARGET_FILE:bkasm> ${PROJECT_DIR}/tests/python/bin/python3 ${PROJECT_DIR}/tests/test_opcodes_errors.py
    COMMENT "Run Tests"
    DEPENDS bkasm setup_venv
)

## opcode generation tests

add_custom_target(test_opcodes
    COMMAND ${CMAKE_COMMAND} -E env BKASM_BINARY=$<TARGET_FILE:bkasm> ${PROJECT_DIR}/tests/python/bin/python3 ${PROJECT_DIR}/tests/test_opcodes.py
    COMMENT "Run Opcode Tests"
    DEPENDS bkasm setup_venv
)

## opcode error tests

add_custom_target(test_opcodes_errors
    COMMAND ${CMAKE_COMMAND} -E env BKASM_BINARY=$<TARGET_FILE:bkasm> ${PROJECT_DIR}/tests/python/bin/python3 ${PROJECT_DIR}/tests/test_opcodes_errors.py
    COMMENT "Run Opcode Error Tests"
    DEPENDS bkasm setup_venv
)

## valgrind test

add_custom_target(valgrind
    COMMAND	valgrind -s --leak-check=full $<TARGET_FILE:bkasm> ${PROJECT_DIR}/tests/test.asm
    COMMENT "Run Memory tests"
    DEPENDS bkasm
)

install(TARGETS bkasm DESTINATION /usr/local/bin)
