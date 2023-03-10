cmake_minimum_required(3.25.3)
project(ls² VERSION 0.0.0)

file(GLOB_RECURSE SRCFILES "src/*.c")

add_executable(ls2 ${SRCFILES})
