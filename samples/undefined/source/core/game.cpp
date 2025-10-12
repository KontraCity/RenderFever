#include "game.hpp"

#include <rf/core/engine.hpp>
#include <rf/graphics/material.hpp>

namespace Undefined {

static void SceneSetup(rf::World::Scene& scene) {
    // The main cube
    rf::World::Entity& mainCube = scene.newEntity("Main Cube");
    mainCube.setComponent<rf::World::DrawComponent>({
        .transform = {},
        .material = {
            .shader = rf::Engine::Library().loadShader("main/"),
            .diffuse = rf::Engine::Library().loadTexture("container/diffuse.png"),
            .specular = rf::Engine::Library().loadTexture("container/specular.png"),
            .shininess = 32.0f,
        },
        .mesh = rf::Engine::Library().loadMesh("cube.glb"),
    });
    mainCube.setComponent<rf::World::LightComponent>({
        .light = {
            .type = rf::Graphics::LightType::PointLight,
            .position = { 0.0f, 0.0f, 0.0f },
            .color = rf::Graphics::VectorColor(50, 0, 0),
        }
    });

    rf::World::Entity& neighbouringPlane = scene.newEntity("Neighbouring Plane");
    neighbouringPlane.setComponent<rf::World::DrawComponent>({
        .transform = {
            .position = { 2.0f, 1.5f, 0.0f },
            .rotation = { 0.0f, -90.0f, 90.0f },
            .scale = { 4.0f, 1.0f, 4.0f }
        },
        .material = {
            .shader = rf::Engine::Library().loadShader("main/"),
            .diffuse = rf::Engine::Library().loadTexture("container/diffuse.png"),
            .specular = rf::Engine::Library().loadTexture("container/specular.png"),
            .shininess = 32.0f,
        },
        .mesh = rf::Engine::Library().loadMesh("plane.glb"),
    });
}

Game::Game() {
    SceneSetup(rf::Engine::Scene());
}

void Game::start() const {
    rf::Engine::Run();
}

} // namespace Undefined
