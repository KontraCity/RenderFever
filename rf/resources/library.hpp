#pragma once

#include <rf/auxiliary/fs.hpp>

#include <rf/resources/manager.hpp>

namespace rf {

namespace Graphics {
    class Shader;
    class Texture;
}

namespace Resources {
    using ShadersManager = Manager<Graphics::Shader>;
    using Shader = ShadersManager::Handle;

    using TexturesManager = Manager<Graphics::Texture>;
    using Texture = TexturesManager::Handle;

    class Library {
    public:
        struct Config {
            fs::path resourcesPath = "resources";
            fs::path shadersPath = "shaders";
            fs::path texturesPath = "textures";
        };

    private:
        ShadersManager::Pointer m_shaders;
        TexturesManager::Pointer m_textures;

    public:
        Library(const Config& config)
            : m_shaders(config.resourcesPath / config.shadersPath)
            , m_textures(config.resourcesPath / config.texturesPath)
        {}

        Library(const Library& other) = delete;

        Library(Library&& other) noexcept = default;

    public:
        Library& operator=(const Library& other) = delete;

        Library& operator=(Library&& other) noexcept = default;

    public:
        Shader loadShader(const fs::path& path) const {
            // Managers are thread safe
            return m_shaders->load(path);
        }

        Texture loadTexture(const fs::path& path) const {
            // Managers are thread safe
            return m_textures->load(path);
        }
    };
}

} // namespace rf

#include <rf/graphics/shader.hpp>
#include <rf/graphics/texture.hpp>
