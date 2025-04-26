add_library(RenderFever STATIC "source/graphics/window.cpp")
target_include_directories(RenderFever PRIVATE "include/")
target_link_libraries(RenderFever PRIVATE ${Dependencies})
