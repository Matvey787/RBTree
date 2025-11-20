# tester.cmake - конфигурация тестового фреймворка

# Tester configuration
set(TEST_PROGRAM_EXEC "${CMAKE_SOURCE_DIR}/build/out")
set(TEST_INPUTS_DIR     "${CMAKE_SOURCE_DIR}/tests/dat")
set(TEST_ANSWERS_DIR    "${CMAKE_SOURCE_DIR}/tests/ans")
set(TESTER_SCRIPT       "${CMAKE_SOURCE_DIR}/tester/tester_script.py")

set(TEST_INPUT_PATTERN  "in")
set(TEST_ANSWER_PATTERN "ans")
set(TEST_PREFIX         "myapp_")
set(TEST_TIMEOUT        30)

