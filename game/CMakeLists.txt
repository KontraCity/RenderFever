add_executable(Game)
add_subdirectory("resources/")
add_subdirectory("source/")
add_dependencies(Game Resources)
