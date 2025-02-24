cmake_minimum_required(VERSION 3.20)
project(RenderFever)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
include_directories("engine/include/")

find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

set(Dependencies
    fmt::fmt
    spdlog::spdlog
    ${OPENGL_LIBRARY}
    GLEW::GLEW
    glfw
    glm::glm
    assimp::assimp
)

add_subdirectory("engine/")
add_subdirectory("games/")
