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
    for (int x = 0; x < 100; x += 10) {
        for (int z = 0; z < 100; z += 10) {
            rf::Entity& cube = scene.newEntity();
            cube.setComponent<rf::DrawComponent>({ { glm::vec3(x, 0.0f, z) }, rf::Meshes::Cube() });
            cube.setComponent<rf::UpdateComponent>({ [](rf::Entity& entity, float deltaTime) {
                auto component = entity.getComponent<rf::DrawComponent>();
                if (component) {
                    float degrees = deltaTime * 90.0f;
                    component->transform.rotation().x += degrees;
                    component->transform.rotation().y += degrees;
                    component->transform.rotation().z += degrees;
                }
            } });
        }
    }
}

Game::Game()
    : m_actionHandle(Bind(Binding::Action, rf::KeyAction::Press, &OnAction))
    , m_escapeHandle(Bind(Binding::Escape, rf::KeyAction::Press | rf::KeyAction::Repeat, &OnEscape)) {
    rf::Scene& scene = rf::Engine::Scene();
    SceneSetup(scene);

    rf::Renderer& renderer = rf::Engine::Renderer();
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
