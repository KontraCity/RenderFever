#pragma once

#include <filesystem>
namespace fs = std::filesystem;

#include "rf/core/asset_manager.hpp"

namespace rf {

class Assets {
public:
    using Shaders = AssetManager<class Shader>;
    using Shader = Shaders::Handle;
    using Textures = AssetManager<class Texture>;
    using Texture = Textures::Handle;

private:
    Shaders m_shaders;
    Textures m_textures;

public:
    Assets(const fs::path& resourcesPath)
        : m_shaders(resourcesPath / "shaders")
        , m_textures(resourcesPath / "textures")
    {}

public:
    Shader loadShader(const fs::path& path) {
        return m_shaders.load(path);
    }

    Texture loadTexture(const fs::path& path) {
        return m_textures.load(path);
    }
};

} // namespace rf
