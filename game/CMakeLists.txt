add_executable(Game "source/main.cpp")
target_include_directories(Game PRIVATE "include/")
target_link_libraries(Game PRIVATE RenderFever ${Dependencies})
