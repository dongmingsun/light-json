cmake_minimum_required(VERSION 3.13)
project(json_tests)

set(CMAKE_CXX_STANDARD 14)
set(GOOGLETEST_VERSION 0.1)

add_subdirectory(../lightjson lightjson)
include_directories(../lightjson)

set(gtest_SOURCE_DIR "googletest")
add_subdirectory(${gtest_SOURCE_DIR})
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(json_tests basic_check.cpp)
target_link_libraries(json_tests lightjson gtest gtest_main)