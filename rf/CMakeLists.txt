add_library(RenderFever STATIC
    "core/engine.cpp"
    "core/fio.cpp"
    "core/image.cpp"
    "core/math.cpp"
    "graphics/mesh.cpp"
    "graphics/meshes.cpp"
    "graphics/renderer.cpp"
    "graphics/shader.cpp"
    "graphics/storage.cpp"
    "graphics/texture.cpp" 
    "graphics/window.cpp"
    "input/input_map.cpp"
    "ui/bindings_hint.cpp"
    "ui/imutil.cpp"
    "ui/overlay.cpp"
)
target_link_libraries(RenderFever PRIVATE ${RfLinkDependencies})
