#pragma once

#include <cstdint>
#include <vector>

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
    using ShadersManager = Manager<Graphics::Shader>;
    using TexturesManager = Manager<Graphics::Texture>;
    using MeshesManager = Manager<Graphics::Mesh>;

    using ShaderId = ShadersManager::ResourceId;
    using TextureId = TexturesManager::ResourceId;
    using MeshId = MeshesManager::ResourceId;

    using Shader = ShadersManager::Handle;
    using Texture = TexturesManager::Handle;
    using Mesh = MeshesManager::Handle;

    class Library {
    public:
        static Graphics::Shader LoadShader(const fs::path& path);

        static Graphics::Texture LoadTexture(const fs::path& path);

        static Graphics::Mesh LoadMesh(const fs::path& path);

        static Graphics::Mesh LoadMesh(const std::vector<uint8_t>& data, const char* formatHint = "");

    private:
        fs::path m_rootDirectory;
        ShadersManager::Pointer m_shaders;
        TexturesManager::Pointer m_textures;
        MeshesManager::Pointer m_meshes;

    public:
        Library(const fs::path& rootDirectory);

        Library(const Library& other) = delete;

        Library(Library&& other) noexcept = default;

    public:
        Library& operator=(const Library& other) = delete;

        Library& operator=(Library&& other) noexcept = default;

    public:
        Shader loadShader(fs::path path);

        Texture loadTexture(fs::path path);

        Mesh loadMesh(fs::path path);

        void reloadShader(const Shader& shader);
        
        void reloadTexture(const Texture& texture);

        void reloadMesh(const Mesh& mesh);

    public:
        const fs::path& rootDirectory() const {
            return m_rootDirectory;
        }

        const ShadersManager& shaders() const {
            return *m_shaders;
        }

        ShadersManager& shaders() {
            return *m_shaders;
        }

        const TexturesManager& textures() const {
            return *m_textures;
        }

        TexturesManager& textures() {
            return *m_textures;
        }

        const MeshesManager& meshes() const {
            return *m_meshes;
        }

        MeshesManager& meshes() {
            return *m_meshes;
        }
    };
}

} // namespace rf

#include <rf/graphics/shader.hpp>
