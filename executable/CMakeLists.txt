## --- Compiler configuration --- ##
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
include_directories("../engine/include/")

## --- Executable configuration  --- ##
add_executable(Executable "main.cpp")
target_link_libraries(Executable PRIVATE RenderFever ${Libraries})
