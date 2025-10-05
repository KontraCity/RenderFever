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
        GLuint vertexArray() const {
            return m_vertexArray;
        }

        size_t indicesCount() const {
            return m_indicesCount;
        }

    public:
        operator bool() const {
            return m_indicesCount != 0;
        }
    };
}

} // namespace rf
