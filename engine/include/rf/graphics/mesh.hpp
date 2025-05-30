#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace rf {

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

    Mesh(const Mesh& other) = delete;

    Mesh(Mesh&& other) noexcept;

    ~Mesh();

public:
    Mesh& operator=(const Mesh& other) = delete;

    Mesh& operator=(Mesh&& other) noexcept;

private:
    void reset();

public:
    GLuint vertexArray() const {
        return m_vertexArray;
    }

    size_t indicesCount() const {
        return m_indicesCount;
    }
};

} // namespace rf
