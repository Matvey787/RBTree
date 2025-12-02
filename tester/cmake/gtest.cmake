enable_testing()

include(FetchContent)

find_package(GTest REQUIRED)

if (NOT GTest_FOUND)
    message(STATUS "Fetching gtest...") 
    FetchContent_Declare(
        googletest
        URL ${GTEST_FETCH_URL}
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    FetchContent_MakeAvailable(googletest)
    find_package(GTest REQUIRED)

else ()
    message(STATUS "Using system GTest: ${GTEST_DIR}")
endif()

file(GLOB GTEST_FILES "${GTEST_DIR}/*.cpp")

add_executable(${GTEST_OUTPUT_EXEC_FILE_NAME} ${GTEST_FILES})

target_link_libraries(${GTEST_OUTPUT_EXEC_FILE_NAME}
    PRIVATE
    GTest::gtest GTest::gtest_main
    ${GTEST_USERDEFINED_ADDITIONAL_LIBS}
)

print_colored_message("GTests generated succesfully, run it by command: \"./build${TESTER_RELATIVE_PATH}/${GTEST_OUTPUT_EXEC_FILE_NAME}\"" 
    ${Color_Bright_Green} ${Bg_Default}
)
