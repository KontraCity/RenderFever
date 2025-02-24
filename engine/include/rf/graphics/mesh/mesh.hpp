#pragma once

#include <vector>

#include "rf/graphics/material.hpp"
#include "rf/graphics/shader.hpp"

namespace rf {

namespace Graphics {
    namespace Mesh {
        struct Vertice {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec2 texCoords;
        };

        using Indice = unsigned int;

        class Mesh {
        private:
            unsigned int m_vertexArray = 0;
            unsigned int m_vertexBuffer = 0;
            unsigned int m_elementBuffer = 0;
            const size_t m_indicesCount = 0;

            Material m_material;

        public:
            Mesh(const std::vector<Vertice>& vertices, const std::vector<Indice>& indices);

            Mesh(const Mesh& other) = delete;

            Mesh(Mesh&& other) noexcept;

            ~Mesh();

        public:
            virtual void draw(Shader& shader) const;

        public:
            inline const Material& material() const {
                return m_material;
            }

            inline Material& material() {
                return m_material;
            }
        };
    }
}

} // namespace rf
