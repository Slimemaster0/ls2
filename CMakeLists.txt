cmake_minimum_required(VERSION 3.25)
project(ls² VERSION 0.1.0)
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPLER "clang++")

file(GLOB_RECURSE SRCFILES "src/*.c")

add_executable(ls2 ${SRCFILES})
