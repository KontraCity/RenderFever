#pragma once

#include "rf/graphics/window.hpp"
#include "rf/input/map.hpp"

namespace rf {

class Engine {
private:
    Graphics::Window m_window;
    Input::Map m_inputMap;

private:
    Engine()
        : m_window("RenderFever Engine", { 1280, 720 })
    {}

    static inline Engine& Instance() {
        static Engine instance;
        return instance;
    }

public:
    static inline Graphics::Window& Window() {
        Engine& engine = Instance();
        return engine.m_window;
    }

    static inline Input::Map& InputMap() {
        Engine& engine = Instance();
        return engine.m_inputMap;
    }
};

} // namespace rf
