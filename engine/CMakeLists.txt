add_library(RenderFever STATIC
    "source/core/engine.cpp"
    "source/core/io.cpp"
    "source/graphics/mesh.cpp"
    "source/graphics/mesh_shapes.cpp"
    "source/graphics/renderer.cpp"
    "source/graphics/shader.cpp"
    "source/graphics/window.cpp"
    "source/inputs/event.cpp"
    "source/inputs/input.cpp"
)
target_include_directories(RenderFever PRIVATE "include/")
target_link_libraries(RenderFever PRIVATE ${Dependencies})
