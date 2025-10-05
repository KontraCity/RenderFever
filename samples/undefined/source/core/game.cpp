#include "game.hpp"

#include <rf/core/engine.hpp>
#include <rf/graphics/material.hpp>
#include <rf/graphics/meshes.hpp>

namespace Undefined {

static void SceneSetup(rf::World::Scene& scene) {
    // The main cube
    rf::World::DrawComponent draw = {
        .shader = rf::Engine::Renderer().shaders().main,
        .transform = {},
        .material = {
            .diffuse = rf::Engine::Library().loadTexture("container/diffuse.png"),
            .specular = rf::Engine::Library().loadTexture("container/specular.png"),
            .shininess = 32.0f,
        },
        .mesh = rf::Graphics::Meshes::Cube(),
    };
    scene.newEntity().setComponent<rf::World::DrawComponent>(std::move(draw));
    
    // Faint red light
    // TODO: Standartize light components!
    rf::Graphics::Light light = {
        .position = { 0.0f, -0.7f, 0.0f },
        .color = rf::Graphics::VectorColor(20, 0, 0),
        .type = rf::Graphics::LightType::PointLight,
    };
    scene.newEntity().setComponent<rf::Graphics::Light>(std::move(light));
}

Game::Game() {
    SceneSetup(rf::Engine::Scene());
}

void Game::start() const {
    rf::Engine::Run();
}

} // namespace Undefined
