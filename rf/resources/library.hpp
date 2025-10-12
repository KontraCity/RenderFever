#pragma once

#include <rf/auxiliary/fs.hpp>

#include <rf/core/image.hpp>
#include <rf/graphics/mesh.hpp>
#include <rf/graphics/texture.hpp>
#include <rf/resources/manager.hpp>

namespace rf {

namespace Graphics {
    class Shader;
}

namespace Resources {
    struct MeshInfo {};
    using MeshesManager = Manager<Graphics::Mesh, MeshInfo>;
    using Mesh = MeshesManager::Handle;

    struct ShaderInfo {
        fs::path vertexFilePath;
        fs::path geometryFilePath;
        fs::path fragmentFilePath;
        bool geometryStage = false;
    };
    using ShadersManager = Manager<Graphics::Shader, ShaderInfo>;
    using Shader = ShadersManager::Handle;

    struct TextureInfo {
        Image::Dimensions dimensions;
    };
    using TexturesManager = Manager<Graphics::Texture, TextureInfo>;
    using Texture = TexturesManager::Handle;

    class Library {
    public:
        struct Config {
            fs::path resourcesPath = "resources";
            fs::path meshesPath = "meshes";
            fs::path shadersPath = "shaders";
            fs::path texturesPath = "textures";
        };

    private:
        fs::path m_meshesBasePath;
        MeshesManager::Pointer m_meshes;

        fs::path m_shadersBasePath;
        ShadersManager::Pointer m_shaders;

        fs::path m_texturesBasePath;
        TexturesManager::Pointer m_textures;

    public:
        Library(const Config& config);

        Library(const Library& other) = delete;

        Library(Library&& other) noexcept = default;

    public:
        Library& operator=(const Library& other) = delete;

        Library& operator=(Library&& other) noexcept = default;

    public:
        Mesh loadMesh(fs::path path);

        Shader loadShader(fs::path path);

        Texture loadTexture(fs::path path);
    };
}

} // namespace rf

#include <rf/graphics/shader.hpp>
