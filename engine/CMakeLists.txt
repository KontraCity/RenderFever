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
    "source/graphics/cube.cpp"
    "source/graphics/material.cpp"
    "source/graphics/mesh.cpp"
    "source/graphics/model.cpp"
    "source/graphics/shader.cpp"
    "source/graphics/texture.cpp"
    "source/graphics/transform.cpp"
    "source/graphics/window.cpp"

    # Graphics lighting modules
    "source/graphics/lighting/directional_light.cpp"
    "source/graphics/lighting/point_light.cpp"
    "source/graphics/lighting/spot_light.cpp"
)
target_link_libraries(RenderFever PRIVATE ${Libraries})
