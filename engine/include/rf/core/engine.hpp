#pragma once

#include "rf/graphics/window.hpp"

namespace rf {

class Engine {
private:
    Graphics::Window m_window;

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
};

} // namespace rf
