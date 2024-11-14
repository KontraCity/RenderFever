## --- Compiler configuration --- ##
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
include_directories("include/rf/")

## --- Engine configuration --- ##
add_library(RenderFever STATIC
    # Common modules
    "source/utility.cpp"
)
