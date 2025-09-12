add_library(RenderFever STATIC
    "core/engine.cpp"
    "core/image.cpp"
    "core/io.cpp"
    "external/stb_image.c"
    "graphics/mesh.cpp"
    "graphics/meshes.cpp"
    "graphics/renderer.cpp"
    "graphics/shader.cpp"
    "graphics/texture.cpp" 
    "graphics/window.cpp"
 )
target_link_libraries(RenderFever PRIVATE ${Dependencies})
