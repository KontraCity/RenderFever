#include "game.hpp"

#include <rf/core/engine.hpp>
#include <rf/graphics/material.hpp>
#include <rf/graphics/meshes.hpp>

#include "utility/random.hpp"

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
    rf::Material material = {};
    material.texture = std::make_shared<rf::Texture>("resources/textures/container/texture.png", rf::Texture::Type::Texture);
    material.specular = std::make_shared<rf::Texture>("resources/textures/container/specular.png", rf::Texture::Type::Specular);
    material.shininess = 32.0f;

    const float scale = 1.25f;
    for (int x = 0; x < 10; x += 1) {
        for (int y = 0; y < 10; y += 1) {
            for (int z = 0; z < 10; z += 1) {
                rf::Entity& cube = scene.newEntity();
                cube.setComponent<rf::DrawComponent>({ { glm::vec3(x * scale, -y * scale, z * scale) }, material, rf::Meshes::Cube() });
                cube.setComponent<rf::UpdateComponent>({ [](rf::Entity& entity, float deltaTime) {
                    auto component = entity.getComponent<rf::DrawComponent>();
                    if (component) {
                        float degrees = deltaTime * 90.0f * Utility::Random(0.0f, 2.0f);
                        component->transform.rotation().x += degrees * Utility::Random(0.0f, 2.0f);
                        component->transform.rotation().y += degrees * Utility::Random(0.0f, 2.0f);
                        component->transform.rotation().z += degrees * Utility::Random(0.0f, 2.0f);
                    }
                } });
            }
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
