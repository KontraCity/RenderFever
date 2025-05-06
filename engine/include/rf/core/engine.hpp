#pragma once

#include <memory>

#include "rf/graphics/renderer.hpp"
#include "rf/graphics/window.hpp"
#include "rf/inputs/map.hpp"

namespace rf {

class Engine {
private:
    static std::unique_ptr<Engine> Instance;

private:
    Graphics::Window m_window;
    Graphics::Renderer m_renderer;
    Inputs::Map m_inputMap;

private:
    Engine();

public:
    static Graphics::Window& Window() {
        return Instance->m_window;
    }
    
    static Graphics::Renderer& Renderer() {
        return Instance->m_renderer;
    }

    static Inputs::Map& InputMap() {
        return Instance->m_inputMap;
    }
};

} // namespace rf
