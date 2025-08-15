add_library(RenderFever STATIC
    "core/engine.cpp"
    "core/io.cpp"
    "graphics/mesh.cpp"
    "graphics/mesh_shapes.cpp"
    "graphics/renderer.cpp"
    "graphics/shader.cpp"
    "graphics/window.cpp"
)
target_link_libraries(RenderFever PRIVATE ${Dependencies})
