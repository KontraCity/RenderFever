#pragma once

#include <vector>

#include <rf/auxiliary/gl.hpp>
#include <rf/auxiliary/glm.hpp>

namespace rf {

namespace Graphics {
    class Mesh {
    public:
        struct Vertice {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec2 texCoords;
        };
        using Indice = GLuint;

    private:
        GLuint m_vertexArray = 0;
        GLuint m_vertexBuffer = 0;
        GLuint m_elementBuffer = 0;
        size_t m_indicesCount = 0;

        // Mean vertices center and radius
        glm::vec3 m_center { 0.0f };
        float m_radius = 0.0f;

    public:
        Mesh(const std::vector<Vertice>& vertices, const std::vector<Indice>& indices);

        Mesh() = default;

        Mesh(const Mesh& other) = delete;

        Mesh(Mesh&& other) noexcept;

        ~Mesh();

    public:
        Mesh& operator=(const Mesh& other) = delete;

        Mesh& operator=(Mesh&& other) noexcept;

    private:
        void free();

    public:
        bool isValid() const {
            return m_vertexArray && m_vertexBuffer && m_elementBuffer && m_indicesCount;
        }

        GLuint vertexArray() const {
            return m_vertexArray;
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
