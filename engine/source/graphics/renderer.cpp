#include "rf/graphics/renderer.hpp"

#include <GL/glew.h>

#include "rf/core/engine.hpp"

namespace rf {

void Renderer::evaluateTimes() const {
    m_time = glfwGetTime();
    static float lastTime = m_time;
    m_deltaTime = m_time - lastTime;
    lastTime = m_time;
}

void Renderer::clearBuffers() const {
    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::run() {
    Window& window = Engine::Window();
    while (!window.shouldClose()) {
        evaluateTimes();
        clearBuffers();

        m_updateDispatcher->broadcast(m_deltaTime);
        Engine::InputMap().update();
        window.update();
    }
}

} // namespace rf
