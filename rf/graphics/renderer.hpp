#pragma once

#include <memory>

#include "rf/core/dispatcher.hpp"
#include "rf/graphics/camera.hpp"
#include "rf/graphics/shader.hpp"
#include "rf/graphics/storage.hpp"

namespace rf {

class Renderer {
public:
    struct Config {
        Shader::Config mainShaderConfig;
        Shader::Config lightShaderConfig;
        size_t lightSourcesReserve = 1000;
    };

private:
    bool m_init = false;
    std::unique_ptr<Shader> m_mainShader;
    std::unique_ptr<Shader> m_lightShader;
    std::unique_ptr<Storage> m_lightStorage;

public:
    Renderer() = default;

private:
    void clear();

    void capture();

    void illuminate();

    void draw();

public:
    void init(const Config& config);

    void render();
};

} // namespace rf
