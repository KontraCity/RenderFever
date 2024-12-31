## --- Compiler configuration --- ##
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
include_directories("include/")

## --- Engine configuration --- ##
add_library(RenderFever STATIC
    # Engine modules
    "source/engine/image.cpp"
    "source/engine/utility.cpp"

    # External modules
    "source/external/stb_image.c"

    # Graphics modules
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

    # Graphics light modules
    "source/graphics/light/directional_light.cpp"
    "source/graphics/light/point_light.cpp"
    "source/graphics/light/spot_light.cpp"

    # Graphics mesh modules
    "source/graphics/mesh/cube.cpp"
    "source/graphics/mesh/mesh.cpp"
    "source/graphics/mesh/plane.cpp"
)
target_link_libraries(RenderFever PRIVATE ${Libraries})
