include(colors.cmake)

print_colored_message("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓" ${Color_Black} ${Bg_Bright_Green})
print_colored_message("┃ NOTE: CHANGE TESTER CONFIGURATION tester/tester_config.cmake ┃" ${Color_Black} ${Bg_Bright_Green})
print_colored_message("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛" ${Color_Black} ${Bg_Bright_Green})

print_colored_message("Tester configuration:"                             ${Color_Bright_Green} ${Bg_Default})
print_colored_message("    Tests directory:         ${TEST_INPUTS_DIR}"   ${Color_Bright_Green} ${Bg_Default})
print_colored_message("    Answers directory:       ${TEST_ANSWERS_DIR}"  ${Color_Bright_Green} ${Bg_Default})
print_colored_message("    Tester script:           ${TESTER_SCRIPT}"     ${Color_Bright_Green} ${Bg_Default})
print_colored_message("    Program executable file: ${TEST_PROGRAM_EXEC}" ${Color_Bright_Green} ${Bg_Default})
include(CTest)
enable_testing()

file(GLOB TEST_INPUTS "${TEST_INPUTS_DIR}/*.${TEST_INPUT_PATTERN}")

function (create_tests)
    foreach(test_input ${TEST_INPUTS})
        get_filename_component(test_name "${test_input}" NAME_WE)

        set(test_answer "${TEST_ANSWERS_DIR}/${test_name}.${TEST_ANSWER_PATTERN}")

        if (EXISTS "${test_answer}")
            add_test(
                NAME "${test_name}"
                COMMAND "python3" "${TESTER_SCRIPT}"
                        "${TEST_PROGRAM_EXEC}"
                        "${test_input}"
                        "${test_answer}"
            )
            set_tests_properties("${test_name}" PROPERTIES
                    TIMEOUT "${TEST_TIMEOUT}"
            )
            message(STATUS "Added test: ${test_name}")
        else()
            message(WARNING "Answer file not found: ${test_answer}")
        endif()
    endforeach()
endfunction()

if(TEST_INPUTS)
    message(STATUS
        "Tests founded in ${TEST_INPUTS_DIR}, creating tests..."
    )
    create_tests()
else()
    message(WARNING
        "No test inputs found in ${TEST_INPUTS_DIR} with pattern *.${TEST_INPUT_PATTERN}"
    )
endif()
