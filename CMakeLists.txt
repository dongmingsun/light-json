# This CMake project is convinient for compiling lightjson library and making some quick verifications
# More precise and strict tests should be handled in the json_test project
cmake_minimum_required(VERSION 3.13)
project(json_main)

set(CMAKE_CXX_STANDARD 14)

include_directories(lightjson)
add_subdirectory(lightjson)
add_executable(json_main main.cpp)
target_link_libraries(json_main lightjson)


