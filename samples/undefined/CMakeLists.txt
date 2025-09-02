add_executable(Undefined)
add_subdirectory("resources/")
add_subdirectory("source/")
add_dependencies(Undefined Resources)
