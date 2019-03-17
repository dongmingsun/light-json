cmake_minimum_required(VERSION 3.13)
project(json_tutorial)

set(CMAKE_CXX_STANDARD 14)

add_library(lightjson STATIC json.cpp JsonValue.cpp)

