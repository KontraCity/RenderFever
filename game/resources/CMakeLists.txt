file(GLOB_RECURSE ResourceFiles
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/*
)

set(FilesToCopy "")
foreach(File ${ResourceFiles})
    if(NOT File STREQUAL "CMakeLists.txt")
        get_filename_component(FileDirectories ${File} DIRECTORY)
        add_custom_command(
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${File}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/${FileDirectories}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${CMAKE_CURRENT_SOURCE_DIR}/${File}
                ${CMAKE_CURRENT_BINARY_DIR}/${File}
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${File}
            COMMENT "Copying resource: ${File}"
        )
        list(APPEND FilesToCopy ${CMAKE_CURRENT_BINARY_DIR}/${File})
    endif()
endforeach()
add_custom_target(Resources ALL DEPENDS ${FilesToCopy})
