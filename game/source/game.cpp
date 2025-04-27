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
        CursorMove,
        Scroll,
        LeftMouseButton,
    };

    rf::Input::Binding& closeBinding = inputMap.createBinding(Bindings::CloseWindow, "Close window");
    closeBinding.dispatcher.subscribe(std::bind(&Game::onCloseWindow, this, _1));
    inputMap.bind(rf::Input::Input::Key_Escape, closeBinding.handle);

    rf::Input::Binding& sayBinding = inputMap.createBinding(Bindings::SayHelloWorld, "Say 'Hello, World!'");
    sayBinding.dispatcher.subscribe(std::bind(&Game::onSayHelloWorld, this, _1));
    inputMap.bind(rf::Input::Input::Key_E, sayBinding.handle);

    rf::Input::Binding& cursorMoveBinding = inputMap.createBinding(Bindings::CursorMove, "Cursor move");
    cursorMoveBinding.dispatcher.subscribe(std::bind(&Game::onCursorMove, this, _1));
    inputMap.bind(rf::Input::Input::Special_CursorMove, cursorMoveBinding.handle);

    rf::Input::Binding& scrollBinding = inputMap.createBinding(Bindings::Scroll, "Scroll");
    scrollBinding.dispatcher.subscribe(std::bind(&Game::onScroll, this, _1));
    inputMap.bind(rf::Input::Input::Special_Scroll, scrollBinding.handle);

    rf::Input::Binding& leftMouseButtonBinding = inputMap.createBinding(Bindings::LeftMouseButton, "Left mouse button");
    leftMouseButtonBinding.dispatcher.subscribe(std::bind(&Game::onLeftMouseButton, this, _1));
    inputMap.bind(rf::Input::Input::Mouse_Button1, leftMouseButtonBinding.handle);
}

void Game::onCloseWindow(const rf::Input::Event& event) {
    if (event.type() == rf::Input::Event::Type::KeyEvent) {
        auto keyEvent = std::get<rf::Input::KeyEvent>(event);
        if (keyEvent == rf::Input::KeyEvent::Press)
            rf::Engine::Window().setShouldClose();
    }
}

void Game::onSayHelloWorld(const rf::Input::Event& event) {
    if (event.type() == rf::Input::Event::Type::KeyEvent) {
        auto keyEvent = std::get<rf::Input::KeyEvent>(event);
        if (keyEvent == rf::Input::KeyEvent::Press)
            fmt::print("Hello, World!\n");
    }
}

void Game::onCursorMove(const rf::Input::Event& event) {
    if (event.type() == rf::Input::Event::Type::CursorMoveEvent) {
        const auto& cursorEvent = std::get<rf::Input::CursorMoveEvent>(event);
        fmt::print("Cursor move: {}x{}!\n", cursorEvent.xPosition, cursorEvent.yPosition);
    }
}

void Game::onScroll(const rf::Input::Event& event) {
    if (event.type() == rf::Input::Event::Type::ScrollEvent) {
        const auto& scrollEvent = std::get<rf::Input::ScrollEvent>(event);
        fmt::print("Scroll: {}x{}!\n", scrollEvent.xOffset, scrollEvent.yOffset);
    }
}

void Game::onLeftMouseButton(const rf::Input::Event& event) {
    if (event.type() == rf::Input::Event::Type::KeyEvent) {
        const auto& keyEvent = std::get<rf::Input::KeyEvent>(event);
        fmt::print("Left mouse button: {}\n", rf::Input::KeyEventName(keyEvent));
    }
}

void Game::run() const {
    rf::Graphics::Window& window = rf::Engine::Window();
    while (!window.shouldClose())
        window.update();
}
