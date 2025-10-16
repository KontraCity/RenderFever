target_sources(Undefined PRIVATE
    "core/binding.cpp"
    "core/game.cpp"
    "main/main.cpp"
)
target_include_directories(Undefined PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(Undefined PRIVATE RenderFever ${RfLinkDependencies})
