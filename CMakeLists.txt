cmake_minimum_required(VERSION 3.19)
project(WORDLE)

set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ standard to conform to")

add_subdirectory(include)
add_subdirectory(src)