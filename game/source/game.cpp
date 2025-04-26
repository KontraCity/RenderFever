#include "game.hpp"

#include <fmt/format.h>

#include <rf/core/engine.hpp>

Game::Game() {
    rf::Graphics::Window& window = rf::Engine::Window();
    window.rename("Game");
    window.resize({ 1920, 1080 });

    rf::Input::Map& inputMap = rf::Engine::InputMap();
    enum class Bindings {
        CloseWindow,
        SayHelloWorld,
    };

    rf::Input::Binding& closeBinding = inputMap.createBinding(Bindings::CloseWindow, "Close window");
    closeBinding.dispatcher.subscribe(std::bind(&Game::closeWindow, this, _1));
    inputMap.bind(rf::Input::Key::Escape, closeBinding.handle);

    rf::Input::Binding& sayBinding = inputMap.createBinding(Bindings::SayHelloWorld, "Say 'Hello, World!'");
    sayBinding.dispatcher.subscribe(std::bind(&Game::sayHelloWorld, this, _1));
    inputMap.bind(rf::Input::Key::E, sayBinding.handle);
}

void Game::closeWindow(rf::Input::Action action) {
    if (action == rf::Input::Action::Press)
        m_stop = true;
}

void Game::sayHelloWorld(rf::Input::Action action) {
    if (action == rf::Input::Action::Press)
        fmt::print("Hello, World!\n");
}

void Game::run() {
    while (!m_stop)
        rf::Engine::Window().update();
}
