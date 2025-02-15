cmake_minimum_required(VERSION 3.30)
project(JsonParser)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_compile_options(-O3 -w)

file(GLOB_RECURSE SRC ./json/Node.cpp ./json/Parser.cpp)

add_executable(JsonParser ${SRC} main.cpp)
