#pragma once

#include <memory>

#include "rf/core/dispatcher.hpp"
#include "rf/graphics/camera.hpp"
#include "rf/graphics/shader.hpp"

namespace rf {

class Renderer {
private:
    std::shared_ptr<Camera> m_camera;
    std::unique_ptr<Shader> m_shader;

public:
    Renderer() = default;

public:
    void useCamera(const std::shared_ptr<Camera> camera) {
        m_camera = camera;
    }

    template<typename... Arguments>
    void useShader(Arguments&&... arguments) {
        m_shader = std::make_unique<Shader>(std::forward<Arguments>(arguments)...);
    }

    void render();
};

} // namespace rf
