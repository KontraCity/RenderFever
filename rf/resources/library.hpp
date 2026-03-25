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
    class Model;
}

namespace Resources {
    using ShadersManager = Manager<Graphics::Shader>;
    using ShaderId = ShadersManager::ResourceId;
    using Shader = ShadersManager::Handle;

    using TexturesManager = Manager<Graphics::Texture>;
    using TextureId = TexturesManager::ResourceId;
    using Texture = TexturesManager::Handle;

    using MeshesManager = Manager<Graphics::Mesh>;
    using MeshId = MeshesManager::ResourceId;
    using Mesh = MeshesManager::Handle;

    using ModelsManager = Manager<Graphics::Model>;
    using ModelId = ModelsManager::ResourceId;
    using Model = ModelsManager::Handle;

    class Library {
    public:
        static Graphics::Shader LoadShader(const fs::path& path);

        static Graphics::Texture LoadTexture(const uint8_t* data, size_t length);

        static Graphics::Texture LoadTexture(const fs::path& path);

        static Graphics::Mesh LoadMesh(const std::vector<uint8_t>& data, const char* formatHint = "");

        static Graphics::Mesh LoadMesh(const fs::path& path);

        static Graphics::Model LoadModel(const std::vector<uint8_t>& data, const char* formatHint = "");

        static Graphics::Model LoadModel(const fs::path& path);
        

    private:
        fs::path m_rootDirectory;
        ShadersManager::Pointer m_shaders;
        TexturesManager::Pointer m_textures;
        MeshesManager::Pointer m_meshes;
        ModelsManager::Pointer m_models;

    public:
        Library(const fs::path& rootDirectory);

        Library(const Library& other) = delete;

        Library(Library&& other) noexcept = default;

        ~Library() = default;

    public:
        Library& operator=(const Library& other) = delete;

        Library& operator=(Library&& other) noexcept = default;

    public:
        Shader loadShader(const fs::path& path);

        Texture loadTexture(const fs::path& path, const uint8_t* data, size_t length);

        Texture loadTexture(const fs::path& path);

        Mesh loadMesh(const fs::path& path);

        Model loadModel(const fs::path& path);

        void reloadShader(const Shader& shader);
        
        void reloadTexture(const Texture& texture);

        void reloadMesh(const Mesh& mesh);

        void reloadModel(const Model& model);

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

        const ModelsManager& models() const {
            return *m_models;
        }

        ModelsManager& models() {
            return *m_models;
        }
    };
}

} // namespace rf

#include <rf/graphics/shader.hpp>
#include <rf/graphics/model.hpp>
