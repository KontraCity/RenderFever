cmake_minimum_required(VERSION 3.20)
project(RenderFever)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

include("cmake/compiler_config.cmake")
include("cmake/dependencies.cmake")

add_subdirectory("rf/")
add_subdirectory("samples/")
