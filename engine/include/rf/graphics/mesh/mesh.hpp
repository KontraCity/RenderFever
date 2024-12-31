#pragma once

// STL modules
#include <vector>

// Custom modules
#include "rf/graphics/material.hpp"
#include "rf/graphics/shader.hpp"

namespace rf {

namespace Graphics
{
    namespace Mesh
    {
        struct Vertice
        {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec2 texCoords;
        };

        using Indice = unsigned int;

        class Mesh
        {
        private:
            /* OpenGL members */
            unsigned int m_vertexArray = 0;
            unsigned int m_vertexBuffer = 0;
            unsigned int m_elementBuffer = 0;
            const size_t m_indicesCount = 0;

            /* Other members */
            Material m_material;

        public:
            /// @brief Create mesh
            /// @param vertices Mesh vertices
            /// @param indices Mesh indices
            Mesh(const std::vector<Vertice>& vertices, const std::vector<Indice>& indices);

            Mesh(const Mesh& other) = delete;

            Mesh(Mesh&& other) noexcept;

            ~Mesh();

        public:
            /// @brief Draw mesh to the screen
            /// @param shader The shader to draw with
            virtual void draw(Shader& shader) const;

        public:
            /// @brief Get mesh material
            /// @return Mesh material
            inline const Material& material() const
            {
                return m_material;
            }

            /// @brief Get mesh material
            /// @return Mesh material
            inline Material& material()
            {
                return m_material;
            }
        };
    }
}

} // namespace rf
