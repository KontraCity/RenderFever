#include "game.hpp"

#include <rf/core/engine.hpp>
#include <rf/graphics/mesh_shapes.hpp>

namespace Game {

Game::Game()
    : m_shader("resources/shaders/vertex/main.vert", "resources/shaders/fragment/main.frag")
    , m_cube(std::make_unique<rf::CubeMesh>(), { glm::vec3(0.0f, -0.3f, 0.0f ) })
    , m_plane(std::make_unique<rf::PlaneMesh>(), { glm::vec3(0.0f, 0.7f, 0.0f), glm::vec3(0.0f, 0.0f, 150.0f) }) {
    rf::Window& window = rf::Engine::Window();
    window.rename("Game");
    window.resize({ 1920, 1080 });
    window.setCursorMode(rf::Window::CursorMode::Disabled);

    m_actionHandle = Bind(Binding::Action, rf::KeyAction::Press, [](rf::KeyAction) {
        auto& window = rf::Engine::Window();
        if (window.cursorMode() == rf::Window::CursorMode::Normal)
            window.setCursorMode(rf::Window::CursorMode::Disabled);
    });

    m_escapeHandle = Bind(Binding::Escape, rf::KeyAction::Press | rf::KeyAction::Repeat, [](rf::KeyAction) {
        auto& window = rf::Engine::Window();
        if (window.cursorMode() == rf::Window::CursorMode::Disabled)
            window.setCursorMode(rf::Window::CursorMode::Normal);
        else if (window.cursorMode() == rf::Window::CursorMode::Normal)
            window.setShouldClose(true);
    });

    // TODO: Get rid of this when proper ECS is implemented.
    m_updateHandle = rf::Engine::Renderer().updateDispatcher()->subscribe([this](float deltaTime) {
        float degrees = deltaTime * 90.0f;
        m_cube.transform().rotation().x += degrees;

        m_shader.capture(m_player.camera());
        m_cube.draw(m_shader);
        m_plane.draw(m_shader);
    });

    ShowBindings();
}

} // namespace Game
