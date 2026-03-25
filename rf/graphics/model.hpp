#pragma once

#include <vector>

#include <rf/auxiliary/glm.hpp>

#include <rf/core/error.hpp>
#include <rf/graphics/mesh.hpp>
#include <rf/graphics/material.hpp>

namespace rf {

namespace Graphics {
    class Model {
    public:
        struct MatMesh {
            int matIndex = -1;
            Mesh mesh;
        };

    private:
        std::vector<MatMesh> m_meshes;
        std::vector<Material> m_materials;

        size_t m_verticesCount = 0;
        size_t m_indicesCount = 0;

        // Mean vertices center and max radius
        glm::vec3 m_center{ 0.0f };
        float m_radius = 0.0f;

    public:
        Model(std::vector<MatMesh>&& meshes, std::vector<Material>&& materials);

        Model() = default;

        Model(const Model& other) = delete;

        Model(Model&& other) noexcept;

        ~Model() = default;

    public:
        Model& operator=(const Model& other) = delete;

        Model& operator=(Model&& other) noexcept;

    public:
        Mesh extractJustAMesh() {
            if (!isJustAMesh())
                throw RF_LOCATED_ERROR("Model is not just a mesh");

            MatMesh mesh = std::move(m_meshes.at(0));
            m_meshes.clear();
            return std::move(mesh.mesh);
        }

    public:
        bool isValid() const {
            return !m_meshes.empty();
        }

        bool isJustAMesh() const {
            return m_meshes.size() == 1;
        }

        const std::vector<MatMesh>& meshes() const {
            return m_meshes;
        }

        const std::vector<Material>& materials() const {
            return m_materials;
        }

        size_t trianglesCount() const {
            return m_indicesCount / 3;
        }

        size_t verticesCount() const {
            return m_verticesCount;
        }

        size_t indicesCount() const {
            return m_indicesCount;
        }

        const glm::vec3& center() const {
            return m_center;
        }

        float radius() const {
            return m_radius;
        }

    public:
        operator bool() const {
            return isValid();
        }
    };
}

} // namespace rf

