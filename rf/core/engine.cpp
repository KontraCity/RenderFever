#include "engine.hpp"

#include <array>

#include <GL/glew.h>

#include <fmt/format.h>

namespace rf {

static float EvaluateDeltaTime(float time) {
    static float lastTime = time;
    float deltaTime = time - lastTime;
    lastTime = time;
    return deltaTime;
}

static void UpdateDeltaTimeDisplay(float time, float deltaTime, Window& window) {
    struct Record { float time = 0.0f; float deltaTime = 0.0f; };
    static std::array<Record, 1000> history{};
    static size_t index = 0;

    history[index++] = { time, deltaTime };
    if (index == history.size())
        index = 0;

    const float updateInterval = 0.5f;
    static float lastUpdateTime = time;
    static std::string windowTitle = window.getTitle();
    if (time - lastUpdateTime > updateInterval) {
        float mean = 0.0f;
        size_t counted = 0;
        for (Record record : history) {
            if (time - record.time < updateInterval) {
                mean += record.deltaTime;
                ++counted;
            }
        }
        float smoothDeltaTime = mean / counted;

        window.setTitle(fmt::format(
            "{} [FT: {:.1f} ms, FPS: {:.1f}]",
            windowTitle,
            smoothDeltaTime * 1000,
            1.0f / smoothDeltaTime
        ));
        lastUpdateTime = time;
    }
}

// std::make_unique requires public constructor, hence here operator new is used instead
std::unique_ptr<Engine> Engine::Instance(new Engine);

void Engine::run() {
    m_scene.start();
    for (size_t frame = 0; !m_window.getShouldClose(); ++frame) {
        float time = glfwGetTime();
        m_deltaTime = EvaluateDeltaTime(time);
        UpdateDeltaTimeDisplay(time, m_deltaTime, m_window);

        m_inputMap.update();
        m_scene.update(m_deltaTime);
        m_renderer.render();
        m_window.swapBuffers();
    }
}

Engine::Engine()
    : m_window("RenderFever Engine", { 1280, 720 })
    , m_assets("resources")
{}

} // namespace rf
