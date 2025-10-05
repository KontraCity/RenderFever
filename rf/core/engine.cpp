#include "engine.hpp"

#include <rf/auxiliary/gl.hpp>

#include <rf/core/error.hpp>
#include <rf/utility/time.hpp>

namespace rf {

static float EvaluateDeltaTime(float time) {
    static float lastTime = time;
    float deltaTime = time - lastTime;
    lastTime = time;
    return deltaTime;
}

// std::make_unique requires public constructor, hence here operator new is used instead
std::unique_ptr<Engine> Engine::Instance;

void Engine::Init(const Config& config) {
    if (!Instance)
        Instance.reset(new Engine(config));
}

void Engine::Run() {
    if (!Instance)
        throw RF_LOCATED_ERROR("Uninitialized engine is being run");
    Instance->run();
}

void Engine::Shutdown() {
    if (Instance)
        Instance.reset(nullptr);
}

void Engine::onEscape() {
    if (m_window.getCursorMode() == Graphics::CursorMode::Normal) {
        m_window.setShouldClose(true);
        return;
    }

    if (m_window.getCursorMode() == Graphics::CursorMode::Disabled) {
        m_window.setCursorMode(Graphics::CursorMode::Normal);
        m_overlay.setMouseDisabled(false);
    }
}

void Engine::onCapture() {
    // Don't capture mouse if the overlay wants to use it.
    if (m_overlay.isMouseWanted())
        return;

    if (m_window.getCursorMode() == Graphics::CursorMode::Normal) {
        m_window.setCursorMode(Graphics::CursorMode::Disabled);
        m_overlay.setMouseDisabled(true);
    }
}

void Engine::run() {
    m_scene.start();
    for (size_t frameIndex = 0; !m_window.getShouldClose(); ++frameIndex) {
        m_frameInfo.index = frameIndex;
        m_frameInfo.deltaTime = EvaluateDeltaTime(glfwGetTime());
        
        glfwPollEvents();
        if (m_window.isMinimized()) {
            Utility::Sleep(10ms);
            continue;
        }

        m_inputMap.update(m_frameInfo.deltaTime);
        m_scene.update(m_frameInfo.deltaTime);
        m_renderer.render();
        m_overlay.render();
        m_window.swapBuffers();
    }
}

Engine::Engine(const Config& config)
    : m_inputMap(std::bind(&Engine::onEscape, this), std::bind(&Engine::onCapture, this))
    , m_library(config.library)
    , m_scene()
    , m_window(config.window)
    , m_renderer(config.renderer, m_library)
    , m_overlay(m_window.handle())
    , m_frameInfo()
{}

} // namespace rf
