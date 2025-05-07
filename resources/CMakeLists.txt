file(GLOB_RECURSE ResourceFiles
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/*
)
set(ResourcesOutputDirectory ${CMAKE_BINARY_DIR}/game)

set(FilesToCopy "")
foreach(File ${ResourceFiles})
    if(NOT File STREQUAL "CMakeLists.txt")
        get_filename_component(FileDirectories ${File} DIRECTORY)
        add_custom_command(
            OUTPUT ${ResourcesOutputDirectory}/${File}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${ResourcesOutputDirectory}/${FileDirectories}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${CMAKE_CURRENT_SOURCE_DIR}/${File}
                ${ResourcesOutputDirectory}/${File}
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${File}
            COMMENT "Copying resource: ${File}"
        )
        list(APPEND FilesToCopy ${ResourcesOutputDirectory}/${File})
    endif()
endforeach()
add_custom_target(Resources ALL DEPENDS ${FilesToCopy})
