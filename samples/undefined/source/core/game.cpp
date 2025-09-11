#include "game.hpp"

#include <rf/core/engine.hpp>
#include <rf/graphics/meshes.hpp>

namespace Undefined {

static void OnAction(rf::KeyAction) {
    auto& window = rf::Engine::Window();
    if (window.getCursorMode() == rf::Window::CursorMode::Normal)
        window.setCursorMode(rf::Window::CursorMode::Disabled);
}

static void OnEscape(rf::KeyAction) {
    auto& window = rf::Engine::Window();
    if (window.getCursorMode() == rf::Window::CursorMode::Disabled)
        window.setCursorMode(rf::Window::CursorMode::Normal);
    else if (window.getCursorMode() == rf::Window::CursorMode::Normal)
        window.setShouldClose(true);
}

static void SceneSetup(rf::Scene& scene) {
    auto cube = scene.world().entity();
    cube.set<rf::Mesh>(rf::Meshes::Cube());
    cube.set<rf::Transform>({ glm::vec3(0.0f, -0.3f, 0.0f) });

    auto plane = scene.world().entity();
    plane.set<rf::Mesh>(rf::Meshes::Plane());
    plane.set<rf::Transform>({ glm::vec3(0.0f, 0.7f, 0.0f), glm::vec3(150.0f, 0.0f, 0.0f) });
}

Game::Game()
    : m_actionHandle(Bind(Binding::Action, rf::KeyAction::Press, &OnAction))
    , m_escapeHandle(Bind(Binding::Escape, rf::KeyAction::Press | rf::KeyAction::Repeat, &OnEscape)) {
    rf::Scene& scene = rf::Engine::Scene();
    SceneSetup(scene);

    rf::Renderer& renderer = rf::Engine::Renderer();
    renderer.useCamera(m_player.camera());
    renderer.useShader("resources/shaders/main.vert", "resources/shaders/main.frag");

    rf::Window& window = rf::Engine::Window();
    window.setTitle("Undefined");
    window.setDimensions({ 1920, 1080 });
    window.setCursorMode(rf::Window::CursorMode::Disabled);
}

void Game::start() const {
    ShowBindings();
    rf::Engine::Run();
}

} // namespace Undefined
