#include "rf/graphics/renderer.hpp"

#include <GL/glew.h>

#include "rf/core/engine.hpp"

namespace rf {

float Renderer::evaluateDeltaTime() const {
    float time = glfwGetTime();
    static float lastTime = time;
    float deltaTime = time - lastTime;
    lastTime = time;
    return deltaTime;
}

void Renderer::clearBuffers() const {
    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::run() {
    Window& window = Engine::Window();
    while (!window.shouldClose()) {
        clearBuffers();
        m_updateDispatcher->broadcast(evaluateDeltaTime());
        window.update();
    }
}

} // namespace rf
