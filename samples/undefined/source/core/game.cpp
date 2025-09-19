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
    material.texture = rf::Engine::Assets().loadTexture("container/texture.png");
    material.specular = rf::Engine::Assets().loadTexture("container/specular.png");
    material.shininess = 32.0f;

    rf::Entity& cube = scene.newEntity();
    cube.setComponent<rf::DrawComponent>({ rf::Engine::Renderer().shaders().main, { glm::vec3(0.0f) }, material, rf::Meshes::Cube()});
    

    rf::Light lightComponent = {};
    lightComponent.type = rf::LightType::PointLight;
    lightComponent.position = { 0.0f, -0.7f, 0.0f };
    lightComponent.color = rf::VectorColor(20, 0, 0);

    rf::Entity& light = scene.newEntity();
    light.setComponent<rf::Light>(std::move(lightComponent));
}

Game::Game()
    : m_actionHandle(Bind(Binding::Action, rf::KeyAction::Press, &OnAction))
    , m_escapeHandle(Bind(Binding::Escape, rf::KeyAction::Press | rf::KeyAction::Repeat, &OnEscape)) {
    rf::Engine::Renderer().init({ "main", "light" });
    SceneSetup(rf::Engine::Scene());

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
