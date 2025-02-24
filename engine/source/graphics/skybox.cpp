#include "rf/graphics/skybox.hpp"

#include <GL/glew.h>

namespace rf {

namespace Model {
    constexpr float Vertices[] = {
        // Front
        // Coordinates          Texture coordinates
        -1.0f,  1.0f, -1.0f,     1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,    -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,     1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,    -1.0f, -1.0f, -1.0f,

        // Right
        // Coordinates          Texture coordinates
        -1.0f,  1.0f,  1.0f,    -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,    -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,    -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,    -1.0f, -1.0f,  1.0f,

        // Back
        // Coordinates          Texture coordinates
         1.0f,  1.0f,  1.0f,    -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,     1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,    -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,     1.0f, -1.0f,  1.0f,

        // Left
        // Coordinates          Texture coordinates
         1.0f,  1.0f, -1.0f,     1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,     1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,     1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,     1.0f, -1.0f, -1.0f,

        // Top
        // Coordinates          Texture coordinates
        -1.0f, -1.0f, -1.0f,    -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,     1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,    -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,     1.0f, -1.0f, -1.0f,

        // Bottom
        // Coordinates          Texture coordinates
        -1.0f,  1.0f,  1.0f,    -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,     1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,    -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,     1.0f,  1.0f,  1.0f,
    };

    constexpr int Indices[] = {
        // Front
         0,  2,  1,
         1,  2,  3,

        // Right
         4,  6,  5,
         5,  6,  7,

        // Back
         8, 10,  9,
         9, 10, 11,

        // Left
        12, 14, 13,
        13, 14, 15,

        // Top
        16, 18, 17,
        17, 18, 19,

        // Bottom
        20, 22, 21,
        21, 22, 23,
    };
}

Graphics::Skybox::Skybox() {
    glGenVertexArrays(1, &m_vertexArray);
    glBindVertexArray(m_vertexArray);

    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Model::Vertices), Model::Vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, reinterpret_cast<void*>(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &m_elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Model::Indices), Model::Indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Graphics::Skybox::Skybox(Skybox&& other) noexcept
    : m_vertexArray(other.m_vertexArray)
    , m_vertexBuffer(other.m_vertexBuffer)
    , m_elementBuffer(other.m_elementBuffer)
    , m_cubemap(other.m_cubemap) {
    other.m_vertexArray = 0;
    other.m_vertexBuffer = 0;
    other.m_elementBuffer = 0;
}

Graphics::Skybox::~Skybox() {
    if (m_elementBuffer)
        glDeleteBuffers(1, &m_elementBuffer);
    if (m_vertexBuffer)
        glDeleteBuffers(1, &m_vertexBuffer);
    if (m_vertexArray)
        glDeleteVertexArrays(1, &m_vertexArray);
}

void Graphics::Skybox::draw(Shader& shader) const {
    shader.set("Cubemap", *m_cubemap, 0);

    glDepthFunc(GL_LEQUAL);
    bool aaWasEnabled = glIsEnabled(GL_MULTISAMPLE);
    if (aaWasEnabled) {
        glDisable(GL_MULTISAMPLE);
    }

    glBindVertexArray(m_vertexArray);
    glDrawElements(GL_TRIANGLES, sizeof(Model::Indices), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
    glDepthFunc(GL_LESS);
    if (aaWasEnabled) {
        glEnable(GL_MULTISAMPLE);
    }
}

} // namespace rf
