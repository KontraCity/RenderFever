#include "game.hpp"

#include <rf/core/math.hpp>

#include "main/settings.hpp"
#include "utility/values.hpp"
using namespace Undefined::Settings;

namespace Undefined {

static rf::Graphics::Camera* GetEntityCamera(rf::World::EntityId entityId) {
    rf::World::Entity* entity = rf::Engine::Scene().getEntity(entityId);
    if (!entity)
        return nullptr;

    rf::World::CameraComponent* cameraComponent = entity->get<rf::World::CameraComponent>();
    return cameraComponent ? &cameraComponent->camera : nullptr;
}

Game::Game(const rf::Engine::Config& config) {
    rf::Engine::Init(config);
    handlesSetup();
    sceneSetup();
}

Game::~Game() {
    rf::Engine::Shutdown();
}

void Game::handlesSetup() {
    m_cursorMoveHandle = rf::Engine::InputMap().cursorMoveDispatcher()->subscribe([this](const rf::Input::CursorMoveAction& action) {
        rf::Graphics::Camera* camera = GetEntityCamera(m_playerEntityId);
        if (!camera)
            return;

        camera->rotation.pitch += action.yOffset / camera->zoom * Sensitivity::Look;
        camera->rotation.yaw += action.xOffset * Sensitivity::Look;
    });

    m_cursorScrollHandle = rf::Engine::InputMap().cursorScrollDispatcher()->subscribe([this](const rf::Input::CursorScrollAction& action) {
        rf::Graphics::Camera* camera = GetEntityCamera(m_playerEntityId);
        if (!camera)
            return;

        camera->zoom += action.yOffset * Sensitivity::Zoom;
        Utility::Limit(camera->zoom, CameraZoom::Min, CameraZoom::Max);
    });

    m_moveForwardHandle = Bind(Binding::MoveForward, rf::Input::KeyAction::Hold, [this](rf::Input::KeyAction, float deltaTime) {
        rf::Graphics::Camera* camera = GetEntityCamera(m_playerEntityId);
        if (!camera)
            return;
        camera->position += deltaTime * movementSpeed() * rf::Math::EvaluateDirection(*camera);
    });

    m_moveBackwardHandle = Bind(Binding::MoveBackward, rf::Input::KeyAction::Hold, [this](rf::Input::KeyAction, float deltaTime) {
        rf::Graphics::Camera* camera = GetEntityCamera(m_playerEntityId);
        if (!camera)
            return;
        camera->position -= deltaTime * movementSpeed() * rf::Math::EvaluateDirection(*camera);
    });

    m_moveLeftHandle = Bind(Binding::MoveLeft, rf::Input::KeyAction::Hold, [this](rf::Input::KeyAction, float deltaTime) {
        rf::Graphics::Camera* camera = GetEntityCamera(m_playerEntityId);
        if (!camera)
            return;
        camera->position -= deltaTime * movementSpeed() * rf::Math::EvaluateRight(*camera);
    });

    m_moveRightHandle = Bind(Binding::MoveRight, rf::Input::KeyAction::Hold, [this](rf::Input::KeyAction, float deltaTime) {
        rf::Graphics::Camera* camera = GetEntityCamera(m_playerEntityId);
        if (!camera)
            return;
        camera->position += deltaTime * movementSpeed() * rf::Math::EvaluateRight(*camera);
    });

    m_moveUpHandle = Bind(Binding::MoveUp, rf::Input::KeyAction::Hold, [this](rf::Input::KeyAction, float deltaTime) {
        rf::Graphics::Camera* camera = GetEntityCamera(m_playerEntityId);
        if (!camera)
            return;
        camera->position += deltaTime * movementSpeed() * rf::Graphics::Camera::Up;
    });

    m_moveDownHandle = Bind(Binding::MoveDown, rf::Input::KeyAction::Hold, [this](rf::Input::KeyAction, float deltaTime) {
        rf::Graphics::Camera* camera = GetEntityCamera(m_playerEntityId);
        if (!camera)
            return;
        camera->position -= deltaTime * movementSpeed() * rf::Graphics::Camera::Up;
    });

    m_moveQuicklyHandle = Bind(Binding::MoveQuickly, rf::Input::KeyAction::Press | rf::Input::KeyAction::Release, [this](rf::Input::KeyAction action, float) {
        if (action == rf::Input::KeyAction::Press)
            m_moveQuickly = true;
        else if (action == rf::Input::KeyAction::Release)
            m_moveQuickly = false;
    });

    m_moveSlowlyHandle = Bind(Binding::MoveSlowly, rf::Input::KeyAction::Press | rf::Input::KeyAction::Release, [this](rf::Input::KeyAction action, float) {
        if (action == rf::Input::KeyAction::Press)
            m_moveSlowly = true;
        else if (action == rf::Input::KeyAction::Release)
            m_moveSlowly = false;
    });

    m_resetHandle = Bind(Binding::ResetPlayer, rf::Input::KeyAction::Press | rf::Input::KeyAction::Repeat, [this](rf::Input::KeyAction action, float) {
        rf::Graphics::Camera* camera = GetEntityCamera(m_playerEntityId);
        if (!camera)
            return;

        if (action == rf::Input::KeyAction::Press)
            camera->zoom = 1.0f;
        else if (action == rf::Input::KeyAction::Repeat)
            resetCamera();
    });

    m_spawnLightHandle = Bind(Binding::SpawnLight, rf::Input::KeyAction::Press, [this](rf::Input::KeyAction, float) {
        rf::World::Scene& scene = rf::Engine::Scene();
        rf::Graphics::Camera* camera = GetEntityCamera(m_playerEntityId);
        if (!camera)
            return;

        rf::World::Entity& spawnedLight = scene.newEntity("Spawned Light");
        spawnedLight.set<rf::World::TransformComponent>({
            .transform = {
                .position = camera->position,
                .rotation = { camera->rotation.pitch, -camera->rotation.yaw, 0.0f },
                .scale = { 0.1f, 0.1f, 0.4f },
            },
        });
        spawnedLight.set<rf::World::MeshDrawComponent>({
            .material = {
                .shader = rf::Engine::Library().loadShader("shaders/light.rfs"),
            },
            .mesh = rf::Engine::Library().loadMesh("meshes/cube.glb"),
        });
        spawnedLight.set<rf::World::LightComponent>({
            .light = {
                .type = rf::Graphics::LightType::SpotLight,
                .position = camera->position,
                .direction = rf::Math::EvaluateDirection(*camera),
                .spotInnerCutoff = 7.0f,
                .spotOuterCutoff = 10.0f,
            }
        });
    });

    m_toggleVSyncHandle = Bind(Binding::ToggleVSync, rf::Input::KeyAction::Press, [this](rf::Input::KeyAction, float) {
        rf::Graphics::Window& window = rf::Engine::Window();
        window.setVSync(!window.getVSync());
    });

    m_toggleWireframeModeHandle = Bind(Binding::ToggleWireframeMode, rf::Input::KeyAction::Press, [this](rf::Input::KeyAction, float) {
        rf::Graphics::Renderer& renderer = rf::Engine::Renderer();
        renderer.setWireframeMode(!renderer.getWireframeMode());
    });

    m_switchProjectionModeHandle = Bind(Binding::SwitchProjectionMode, rf::Input::KeyAction::Press, [this](rf::Input::KeyAction, float) {
        rf::Graphics::Camera* camera = GetEntityCamera(m_playerEntityId);
        if (!camera)
            return;

        if (camera->projectionMode == rf::Graphics::ProjectionMode::Perspective)
            camera->projectionMode = rf::Graphics::ProjectionMode::Orthographic;
        else
            camera->projectionMode = rf::Graphics::ProjectionMode::Perspective;
    });

    m_switchToStandaloneCameraHandle = Bind(Binding::SwitchToStandaloneCamera, rf::Input::KeyAction::Press, [this](rf::Input::KeyAction, float) {
        rf::World::Scene& scene = rf::Engine::Scene();
        rf::World::Entity* playerEntity = scene.getEntity(m_playerEntityId);
        rf::World::Entity* standaloneCameraEntity = scene.getEntity(m_standaloneCameraEntityId);
        if (!playerEntity || !standaloneCameraEntity)
            return;

        static bool s_standaloneCameraActive = false;
        if (!s_standaloneCameraActive) {
            scene.setActiveCameraEntity(*standaloneCameraEntity);
            s_standaloneCameraActive = true;
        }
        else {
            scene.setActiveCameraEntity(*playerEntity);
            s_standaloneCameraActive = false;
        }
    });
}

void Game::sceneSetup() {
    rf::Resources::Library& library = rf::Engine::Library();
    rf::World::Scene& scene = rf::Engine::Scene();

    rf::World::Entity& player = scene.newEntity("Player");
    m_playerEntityId = player.id();
    player.add<rf::World::CameraComponent>();
    player.set<rf::World::TransformComponent>({
        .transform = {
            .scale = { 0.4f, 1.0f, 0.4f }
        },
    });
    player.set<rf::World::MeshDrawComponent>({
        .material = {
            .shader = library.loadShader("shaders/main.rfs"),
        },
        .mesh = library.loadMesh("meshes/cube.glb"),
    });
    player.set<rf::World::LogicComponent>({
        .onUpdate = [](rf::World::Entity& entity, float) {
            rf::World::CameraComponent* cameraComponent = entity.get<rf::World::CameraComponent>();
            rf::World::TransformComponent* transformComponent = entity.get<rf::World::TransformComponent>();
            if (cameraComponent && transformComponent) {
                transformComponent->transform.position = cameraComponent->camera.position;
                transformComponent->transform.rotation.yaw = cameraComponent->camera.rotation.yaw;
            }
        }
    });
    scene.setActiveCameraEntity(player);

    rf::World::Entity& standaloneCamera = scene.newEntity("Standalone Camera");
    m_standaloneCameraEntityId = standaloneCamera.id();
    standaloneCamera.set<rf::World::CameraComponent>({
        .camera = {
            .projectionMode = rf::Graphics::ProjectionMode::Perspective,
            .position = { 5.0f, 7.5f, 0.0f },
            .rotation = { -40.0f, -90.0f, 0.0f },
            .zoom = 1.0f,
        }
    });

    rf::World::Entity& fillLight = scene.newEntity("Fill Light");
    fillLight.set<rf::World::TransformComponent>({
        .transform = {
            .position = { 3.5f, 7.0f, 0.0f },
            .rotation = { -50.0f, 90.0f, 0.0f },
            .scale = { 0.1f, 0.1f, 0.4f },
        },
    });
    fillLight.set<rf::World::MeshDrawComponent>({
        .material = {
            .shader = library.loadShader("shaders/light.rfs"),
        },
        .mesh = library.loadMesh("meshes/cube.glb"),
    });
    fillLight.set<rf::World::LightComponent>({
        .light = {
            .type = rf::Graphics::LightType::SpotLight,
            .ambientProperty = 0.5f,
            .diffuseProperty = 1.5f,
            .specularProperty = 2.0f,

            .position = { 3.5f, 7.0f, 0.0f },
            .direction = { -0.643f, -0.766, 0.0f },

            .constantAttenuation = 0.0f,
            .linearAttenuation = 0.0f,
            .quadraticAttenuation = 0.015f,

            .spotInnerCutoff = 10.0f,
            .spotOuterCutoff = 15.0f,
        }
    });

    rf::World::Entity& mainCube = scene.newEntity("Main Cube");
    mainCube.add<rf::World::TransformComponent>();
    mainCube.set<rf::World::MeshDrawComponent>({
        .material = {
            .shader = library.loadShader("shaders/main.rfs"),
            .diffuse = library.loadTexture("textures/container_diffuse.png"),
            .specular = library.loadTexture("textures/container_specular.png"),
            .shininess = 32.0f,
        },
        .mesh = library.loadMesh("meshes/cube.glb"),
    });

    rf::World::Entity& modelEntity = scene.newEntity("Model Entity");
    modelEntity.set<rf::World::TransformComponent>({
        .transform = {
            .position = { 0.0f, 0.49f, 0.0f },
            .rotation = { 0.0f, -90.0f, 0.0f },
        },
    });
    modelEntity.set<rf::World::ModelDrawComponent>({
        .model = library.loadModel("models/sonic.glb"),
    });

    rf::World::Entity& neighbouringPlane = scene.newEntity("Neighbouring Plane");
    neighbouringPlane.set<rf::World::TransformComponent>({
        .transform = {
            .position = { -2.0f, 1.5f, 0.0f },
            .rotation = { 90.0f, 0.0f, 90.0f },
            .scale = { 4.0f, 1.0f, 4.0f }
        },
    });
    neighbouringPlane.set<rf::World::MeshDrawComponent>({
        .material = {
            .shader = library.loadShader("shaders/main.rfs"),
            .diffuse = library.loadTexture("textures/container_diffuse.png"),
            .specular = library.loadTexture("textures/container_specular.png"),
            .shininess = 32.0f,
        },
        .mesh = library.loadMesh("meshes/plane.glb"),
    });
}

float Game::movementSpeed() const {
    if (!m_moveSlowly && m_moveQuickly)
        return MovementSpeed::Quick;
    else if (m_moveSlowly && !m_moveQuickly)
        return MovementSpeed::Slow;
    return MovementSpeed::Normal;
}

void Game::resetCamera() const {
    rf::Graphics::Camera* camera = GetEntityCamera(m_playerEntityId);
    if (!camera)
        return;

    camera->projectionMode = rf::Graphics::ProjectionMode::Perspective;
    camera->position = { 1.30656278f, 1.30656278f, 1.30656278f };
    camera->rotation = { -35.2643890f, -45.0f, 0.0f };
    camera->zoom = 1.0f;
}

void Game::start() const {
    resetCamera();
    rf::Engine::Run();
}

} // namespace Undefined
