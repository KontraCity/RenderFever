#pragma once

// STL modules
#include <string>
#include <vector>
#include <map>
#include <filesystem>

// Library Assimp
#include <assimp/scene.h>

// Custom modules
#include "rf/graphics/mesh.hpp"
#include "rf/graphics/shader.hpp"
#include "rf/graphics/transform.hpp"

namespace rf {
    
/* Namespace aliases and imports */
namespace fs = std::filesystem;

namespace Graphics
{
    class Model
    {
    private:
        /* Model specific */
        fs::path m_directory;
        std::vector<Mesh> m_meshes;
        std::map<std::string, Texture::Pointer> m_textures;

        /* Variables */
        Transform m_transform;

    public:
        Model() = default;

        Model(const Model& other) = delete;

        Model(Model&& other) noexcept = delete;

    private:
        /// @brief Process model node
        /// @param scene Model scene
        /// @param node The node to process
        void processNode(const aiScene* scene, aiNode* node);

        /// @brief Create model mesh
        /// @param scene Model scene
        /// @param mesh The mesh to create
        void createMesh(const aiScene* scene, aiMesh* mesh);

        /// @brief Load mesh textures
        /// @param scene Model scene
        /// @param material Mesh material
        /// @param type Textures type
        /// @param textures Texture array to load to
        void loadTextures(const aiScene* scene, aiMaterial* material, aiTextureType type, std::vector<Texture::Pointer>& textures);

    public:
        /// @brief Load model
        /// @param modelFilePath Path to the model file
        void load(const std::string& modelFilePath);

        /// @brief Draw mesh to the screen
        /// @param shader The shader to draw with
        void draw(Shader& shader) const;

    public:
        /// @brief Get model transform
        /// @return Model transform
        inline const Transform& transform() const
        {
            return m_transform;
        }

        /// @brief Get model transform
        /// @return Model transform
        inline Transform& transform()
        {
            return m_transform;
        }
    };
}

} // namespace rf
