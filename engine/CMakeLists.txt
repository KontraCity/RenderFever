## --- Compiler configuration --- ##
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
include_directories("include/")

## --- Engine configuration --- ##
add_library(RenderFever STATIC
    # Engine modules
    "source/engine/utility.cpp"

    # Graphics modules
    "source/graphics/camera.cpp"
    "source/graphics/cube.cpp"
    "source/graphics/shader.cpp"
    "source/graphics/transform.cpp"
    "source/graphics/window.cpp"
)
target_link_libraries(RenderFever PRIVATE ${Libraries})
