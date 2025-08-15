target_sources(Game PRIVATE
    "core/binding.cpp"
    "core/game.cpp"
    "core/player.cpp"
    "main/main.cpp"
)
target_include_directories(Game PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(Game PRIVATE RenderFever ${Dependencies})
