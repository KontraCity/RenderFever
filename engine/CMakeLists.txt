add_library(RenderFever STATIC
    "source/core/image.cpp"
    "source/core/utility.cpp"

    "source/external/stb_image.c"

    "source/graphics/light/directional_light.cpp"
    "source/graphics/light/point_light.cpp"
    "source/graphics/light/spot_light.cpp"
    "source/graphics/mesh/cube.cpp"
    "source/graphics/mesh/mesh.cpp"
    "source/graphics/mesh/plane.cpp"
    "source/graphics/camera.cpp"
    "source/graphics/color.cpp"
    "source/graphics/cubemap.cpp"
    "source/graphics/material.cpp"
    "source/graphics/mesh/model.cpp"
    "source/graphics/shader.cpp"
    "source/graphics/skybox.cpp"
    "source/graphics/texture.cpp"
    "source/graphics/transform.cpp"
    "source/graphics/window.cpp"
)
target_link_libraries(RenderFever PRIVATE ${Dependencies})
