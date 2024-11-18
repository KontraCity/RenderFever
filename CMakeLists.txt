## --- Project configuration --- ##
cmake_minimum_required(VERSION 3.20)
project(RenderFever)

## --- Packages configuration --- ##
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

## --- Libraries configuration --- ##
set(Libraries
    fmt::fmt
    spdlog::spdlog
    ${OPENGL_LIBRARY}
    GLEW::GLEW
    glfw
    glm::glm
)

## --- Engine and Executable configuration --- ##
add_subdirectory("engine/")
add_subdirectory("executable/")
