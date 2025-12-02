cmake_minimum_required(VERSION 3.11)
project(RedBlackTree CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(GTest REQUIRED)

# -------------------- Red-Black Tree library --------------------
set(RBT_LIB rbt INTERFACE)
add_library(${RBT_LIB})

target_include_directories(${RBT_LIB} INTERFACE ${CMAKE_SOURCE_DIR}/RBT_Lib/inc)
    
# -----------------------------------------------------------------

# -------------------- My RBTree Exec --------------------
add_executable(out 
    src/main.cpp
)

target_link_libraries(out 
    PRIVATE
        ${RBT_LIB}
)
# --------------------------------------------------------

# -------------------- std::set --------------------
add_executable(set
    src/main.cpp
)

target_compile_definitions(set
    PRIVATE
    SET_M
)
# --------------------------------------------------

add_subdirectory(tester)
   