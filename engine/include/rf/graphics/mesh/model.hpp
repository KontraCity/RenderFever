#pragma once

#include <string>
#include <vector>
#include <map>
#include <filesystem>
namespace fs = std::filesystem;

#include <assimp/scene.h>

#include "rf/graphics/mesh/mesh.hpp"
#include "rf/graphics/shader.hpp"
#include "rf/graphics/transform.hpp"

namespace rf {

namespace Graphics {
    namespace Mesh {
        class Model {
        private:
            const fs::path m_directory;
            std::vector<Mesh> m_meshes;
            std::map<std::string, Texture::Pointer> m_textures;

            Transform m_transform;

        public:
            Model(const std::string& modelFilePath);

            Model(const Model& other) = delete;

            Model(Model&& other) noexcept = delete;

        private:
            void processNode(const aiScene* scene, aiNode* node);

            void createMesh(const aiScene* scene, aiMesh* mesh);

            void loadTextures(const aiScene* scene, aiMaterial* meshMaterial, aiTextureType type, Material& material);

        public:
            void draw(Shader& shader) const;

        public:
            inline const Transform& transform() const {
                return m_transform;
            }

            inline Transform& transform() {
                return m_transform;
            }
        };
    }
}

} // namespace rf
