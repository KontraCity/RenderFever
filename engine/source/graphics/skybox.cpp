#include "rf/graphics/skybox.hpp"

// Graphics libraries
#include <GL/glew.h>

namespace Model
{
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

namespace rf {

Graphics::Skybox::Skybox()
{
    // Vertex array object
    glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);

    // Vertex buffer object
    glGenBuffers(1, &m_vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Model::Vertices), Model::Vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, reinterpret_cast<void*>(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    // Element buffer object
    glGenBuffers(1, &m_elementBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Model::Indices), Model::Indices, GL_STATIC_DRAW);
    glBindVertexArray(0);
}

Graphics::Skybox::Skybox(Skybox&& other) noexcept
    : m_vertexArrayObject(other.m_vertexArrayObject)
    , m_vertexBufferObject(other.m_vertexBufferObject)
    , m_elementBufferObject(other.m_elementBufferObject)
    , m_cubemap(other.m_cubemap)
{
    other.m_vertexArrayObject = 0;
    other.m_vertexBufferObject = 0;
    other.m_elementBufferObject = 0;
    other.m_cubemap = {};
}

Graphics::Skybox::~Skybox()
{
    if (m_elementBufferObject)
        glDeleteBuffers(1, &m_elementBufferObject);
    if (m_vertexBufferObject)
        glDeleteBuffers(1, &m_vertexBufferObject);
    if (m_vertexArrayObject)
        glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void Graphics::Skybox::draw(Shader& shader) const
{
    // Apply
    shader.set("Cubemap", *m_cubemap, 0);

    // Prepare configuration
    glDepthFunc(GL_LEQUAL);
    bool aaWasEnabled = glIsEnabled(GL_MULTISAMPLE);
    if (aaWasEnabled)
        glDisable(GL_MULTISAMPLE);

    // Draw
    glBindVertexArray(m_vertexArrayObject);
    glDrawElements(GL_TRIANGLES, sizeof(Model::Indices), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
    // Reset configuration
    glDepthFunc(GL_LESS);
    if (aaWasEnabled)
        glEnable(GL_MULTISAMPLE);
}

} // namespace rf
