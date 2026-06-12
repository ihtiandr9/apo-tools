
## create python venv for tests if missing (not tracked in git — see root .gitignore)

add_custom_target(setup_venv
    COMMAND test -f ${PROJECT_DIR}/tests/python/bin/python3 || python3 -m venv --without-pip ${PROJECT_DIR}/tests/python
    COMMENT "Setup Python venv for tests"
)

## unittest tests one unit

add_custom_target(unittest
    COMMAND	$<TARGET_FILE:bkasm> ${PROJECT_DIR}/tests/test.asm
    COMMENT "Run Test"
    DEPENDS bkasm
)

## legacy error-table tests (deprecated)

add_custom_target(test_errors
    COMMAND ${CMAKE_COMMAND} -E env BKASM_BINARY=$<TARGET_FILE:bkasm> ${PROJECT_DIR}/tests/python/bin/python3 ${PROJECT_DIR}/tests/tests.py
    COMMENT "Run Legacy Error Tests"
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

## DB string generation tests

add_custom_target(test_db_strings
    COMMAND ${CMAKE_COMMAND} -E env BKASM_BINARY=$<TARGET_FILE:bkasm> ${PROJECT_DIR}/tests/python/bin/python3 ${PROJECT_DIR}/tests/test_db_strings.py
    COMMENT "Run DB String Tests"
    DEPENDS bkasm setup_venv
)

## ORG/EQU/DS range tests

add_custom_target(test_org_equ_range
    COMMAND ${CMAKE_COMMAND} -E env BKASM_BINARY=$<TARGET_FILE:bkasm> ${PROJECT_DIR}/tests/python/bin/python3 ${PROJECT_DIR}/tests/test_org_equ_range.py
    COMMENT "Run ORG/EQU/DS Tests"
    DEPENDS bkasm setup_venv
)

## INCLUDE directive tests

add_custom_target(test_include
    COMMAND ${CMAKE_COMMAND} -E env BKASM_BINARY=$<TARGET_FILE:bkasm> ${PROJECT_DIR}/tests/python/bin/python3 ${PROJECT_DIR}/tests/test_include.py
    COMMENT "Run INCLUDE Tests"
    DEPENDS bkasm setup_venv
)

## script unittests (all python tests)

add_custom_target(run_tests
    COMMAND ${CMAKE_COMMAND} -E env BKASM_BINARY=$<TARGET_FILE:bkasm> ${PROJECT_DIR}/tests/python/bin/python3 ${PROJECT_DIR}/tests/tests.py
    COMMAND ${CMAKE_COMMAND} -E env BKASM_BINARY=$<TARGET_FILE:bkasm> ${PROJECT_DIR}/tests/python/bin/python3 ${PROJECT_DIR}/tests/test_opcodes.py
    COMMAND ${CMAKE_COMMAND} -E env BKASM_BINARY=$<TARGET_FILE:bkasm> ${PROJECT_DIR}/tests/python/bin/python3 ${PROJECT_DIR}/tests/test_opcodes_errors.py
    COMMAND ${CMAKE_COMMAND} -E env BKASM_BINARY=$<TARGET_FILE:bkasm> ${PROJECT_DIR}/tests/python/bin/python3 ${PROJECT_DIR}/tests/test_db_strings.py
    COMMAND ${CMAKE_COMMAND} -E env BKASM_BINARY=$<TARGET_FILE:bkasm> ${PROJECT_DIR}/tests/python/bin/python3 ${PROJECT_DIR}/tests/test_org_equ_range.py
    COMMAND ${CMAKE_COMMAND} -E env BKASM_BINARY=$<TARGET_FILE:bkasm> ${PROJECT_DIR}/tests/python/bin/python3 ${PROJECT_DIR}/tests/test_include.py
    COMMENT "Run All Tests"
    DEPENDS bkasm setup_venv
)

## valgrind test

add_custom_target(valgrind
    COMMAND	valgrind -s --leak-check=full $<TARGET_FILE:bkasm> ${PROJECT_DIR}/tests/test.asm
    COMMENT "Run Memory tests"
    DEPENDS bkasm
)

## deprecated — kept for backward compatibility
add_custom_target(tests
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target run_tests
    COMMENT "Run All Tests (deprecated: use run_tests)"
)
