#pragma once

#include <memory>

#include "rf/core/assets.hpp"
#include "rf/graphics/renderer.hpp"
#include "rf/graphics/window.hpp"
#include "rf/inputs/input_map.hpp"
#include "rf/world/scene.hpp"

namespace rf {

class Engine {
private:
    static std::unique_ptr<Engine> Instance;

private:
    float m_deltaTime = 0.0f;
    Assets m_assets;
    Scene m_scene;
    Window m_window;
    Renderer m_renderer;
    InputMap m_inputMap;

private:
    void run();

private:
    Engine();

public:
    static void Run() {
        Instance->run();
    }

    static float DeltaTime() {
        return Instance->m_deltaTime;
    }

    static Assets& Assets() {
        return Instance->m_assets;
    }

    static Scene& Scene() {
        return Instance->m_scene;
    }

    static Window& Window() {
        return Instance->m_window;
    }
    
    static Renderer& Renderer() {
        return Instance->m_renderer;
    }

    static InputMap& InputMap() {
        return Instance->m_inputMap;
    }
};

} // namespace rf
