#pragma once

#include <memory>
#include <filesystem>
namespace fs = std::filesystem;

#include "rf/core/dispatcher.hpp"
#include "rf/core/assets.hpp"
#include "rf/graphics/camera.hpp"
#include "rf/graphics/shader.hpp"
#include "rf/graphics/storage.hpp"

namespace rf {

class Renderer {
public:
    struct Config {
        fs::path mainShaderPath;
        fs::path lightShaderPath;
        size_t lightSourcesReserve = 1000;
    };

    struct Shaders {
        Assets::Shader main;
        Assets::Shader light;
    };

private:
    bool m_init = false;
    Shaders m_shaders;
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

public:
    Shaders shaders() const {
        return m_shaders;
    }
};

} // namespace rf
