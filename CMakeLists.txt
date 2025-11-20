cmake_minimum_required(VERSION 3.11)
project(LIRS_Cache CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(GTest REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/inc)

set(COMMON_SOURCE_FILES
    src/main.cpp
)

add_executable(out ${COMMON_SOURCE_FILES})
target_include_directories(out PRIVATE ${CMAKE_SOURCE_DIR}/inc)

add_executable(generateTest src/generateTest.cpp)

add_executable(set ${COMMON_SOURCE_FILES})
target_compile_definitions(set PRIVATE SET_M)

add_executable(rbt_tests tests/test_rbt.cpp)
target_link_libraries(rbt_tests GTest::gtest GTest::gtest_main)
target_include_directories(rbt_tests PRIVATE ${CMAKE_SOURCE_DIR}/inc)

include(tester/tester.cmake)
   