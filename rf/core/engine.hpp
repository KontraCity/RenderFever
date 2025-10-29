#pragma once

#include <memory>

#include <rf/graphics/renderer.hpp>
#include <rf/graphics/window.hpp>
#include <rf/input/input_map.hpp>
#include <rf/resources/library.hpp>
#include <rf/ui/overlay.hpp>
#include <rf/world/scene.hpp>

namespace rf {

class Engine {
public:
    struct Config {
        Graphics::Renderer::Config renderer;
        Graphics::Window::Config window;
        fs::path resourcesRootDirectory;
    };

    struct FrameInfo {
        size_t index    = 0;
        float time      = 0.0f;
        float deltaTime = 0.0f;
    };

private:
    static std::unique_ptr<Engine> Instance;

public:
    static void Init(const Config& config);

    static void Run();

    static void Shutdown();

private:
    Input::InputMap m_inputMap;
    Resources::Library m_library;
    World::Scene m_scene;
    Graphics::Window m_window;
    Graphics::Renderer m_renderer;
    Ui::Overlay m_overlay;
    FrameInfo m_frameInfo;

private:
    // Engine should only be created via Engine::Init()!
    Engine(const Config& config);

private:
    void onEscape();

    void onCapture();

    void run();

public:
    static Input::InputMap& InputMap() {
        return Instance->m_inputMap;
    }

    static Resources::Library& Library() {
        return Instance->m_library;
    }

    static World::Scene& Scene() {
        return Instance->m_scene;
    }

    static Graphics::Window& Window() {
        return Instance->m_window;
    }

    static Graphics::Renderer& Renderer() {
        return Instance->m_renderer;
    }

    static Ui::Overlay& Overlay() {
        return Instance->m_overlay;
    }

    static const FrameInfo& GetFrameInfo() {
        return Instance->m_frameInfo;
    }
};

} // namespace rf
