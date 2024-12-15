## --- Compiler configuration --- ##
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
include_directories("${CMAKE_SOURCE_DIR}/engine/include/")

## --- Executable configuration  --- ##
add_executable(Executable "main.cpp")
target_link_libraries(Executable PRIVATE RenderFever ${Libraries})

## --- Commands configuration --- ##
add_custom_command(
    TARGET Executable 
    PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:Executable>
)
