#pragma once

// STL modules
#include <string>
#include <vector>
#include <map>
#include <filesystem>

// Library Assimp
#include <assimp/scene.h>

// Custom modules
#include "rf/graphics/mesh/mesh.hpp"
#include "rf/graphics/shader.hpp"
#include "rf/graphics/transform.hpp"

namespace rf {
    
/* Namespace aliases and imports */
namespace fs = std::filesystem;

namespace Graphics
{
    namespace Mesh
    {
        class Model
        {
        private:
            /* Model members */
            const fs::path m_directory;
            std::vector<Mesh> m_meshes;
            std::map<std::string, Texture::Pointer> m_textures;

            /* Other members */
            Transform m_transform;

        public:
            /// @brief Load model from file
            /// @param modelFilePath Path to the model file
            /// @throw std::runtime_error if the model couldn't be loaded
            Model(const std::string& modelFilePath);

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

            /// @brief Load textures to material
            /// @param scene Model scene
            /// @param meshMaterial Mesh material
            /// @param type Textures type
            /// @param material Material to load to
            void loadTextures(const aiScene* scene, aiMaterial* meshMaterial, aiTextureType type, Material& material);

        public:
            /// @brief Draw model to the screen
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
}

} // namespace rf
