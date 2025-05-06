add_executable(Game
    "source/binding.cpp"
    "source/game.cpp"
    "source/main.cpp"
    "source/player.cpp"
)
target_include_directories(Game PRIVATE "include/")
target_link_libraries(Game PRIVATE RenderFever ${Dependencies})
