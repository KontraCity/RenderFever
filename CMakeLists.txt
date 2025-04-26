cmake_minimum_required(VERSION 3.20)
project(RenderFever)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
include_directories("engine/include/")

add_subdirectory("engine/")
add_subdirectory("game/")
