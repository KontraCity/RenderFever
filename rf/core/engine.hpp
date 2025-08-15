#pragma once

#include <memory>

#include "rf/graphics/renderer.hpp"
#include "rf/graphics/window.hpp"
#include "rf/inputs/input_map.hpp"

namespace rf {

class Engine {
private:
    static std::unique_ptr<Engine> Instance;

private:
    Window m_window;
    Renderer m_renderer;
    InputMap m_inputMap;

private:
    Engine();

public:
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
