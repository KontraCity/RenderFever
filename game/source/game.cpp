#include "game.hpp"

#include <rf/core/engine.hpp>
#include <rf/graphics/mesh_shapes.hpp>

namespace Game {

Game::Game()
    : m_actionBinding(Binding::Type::Action, std::bind(&Game::onAction, this, _1))
    , m_escapeBinding(Binding::Type::Escape, std::bind(&Game::onEscape, this, _1))
    , m_shader("resources/shaders/vertex/main.vert", "resources/shaders/fragment/main.frag")
    , m_cube(std::make_unique<rf::Graphics::CubeMesh>(), { glm::vec3(0.0f, -0.3f, 0.0f ) })
    , m_plane(std::make_unique<rf::Graphics::PlaneMesh>(), { glm::vec3(0.0f, 0.7f, 0.0f), glm::vec3(0.0f, 0.0f, 150.0f) })
    , m_updateHandle(rf::Engine::Renderer().updateDispatcher()->subscribe(std::bind(&Game::onUpdate, this, _1))) {
    rf::Graphics::Window& window = rf::Engine::Window();
    window.rename("Game");
    window.resize({ 1920, 1080 });
    window.setCursorMode(rf::Graphics::Window::CursorMode::Disabled);
    Binding::PrintBindings();
}

void Game::onUpdate(float deltaTime) {
    float degrees = deltaTime * 90.0f;
    m_cube.transform().rotation().x += degrees;

    m_shader.capture(m_player.camera());
    m_cube.draw(m_shader);
    m_plane.draw(m_shader);
}

void Game::onAction(const rf::Inputs::Event& event) {
    if (event.type() != rf::Inputs::Event::Type::KeyEvent)
        return;

    auto keyEvent = std::get<rf::Inputs::KeyEvent>(event);
    if (keyEvent != rf::Inputs::KeyEvent::Press)
        return;

    auto& window = rf::Engine::Window();
    if (window.cursorMode() == rf::Graphics::Window::CursorMode::Normal)
        window.setCursorMode(rf::Graphics::Window::CursorMode::Disabled);
}

void Game::onEscape(const rf::Inputs::Event& event) {
    if (event.type() != rf::Inputs::Event::Type::KeyEvent)
        return;

    auto keyEvent = std::get<rf::Inputs::KeyEvent>(event);
    if (keyEvent != rf::Inputs::KeyEvent::Press)
        return;

    auto& window = rf::Engine::Window();
    if (window.cursorMode() == rf::Graphics::Window::CursorMode::Disabled)
        window.setCursorMode(rf::Graphics::Window::CursorMode::Normal);
    else if (window.cursorMode() == rf::Graphics::Window::CursorMode::Normal)
        window.setShouldClose(true);
}

} // namespace Game
