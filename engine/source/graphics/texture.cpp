#include "rf/graphics/texture.hpp"

// STL modules
#include <stdexcept>

// Graphics libraries
#include <GL/glew.h>

namespace rf {

/// @brief Load texture from image
/// @param The image to load the texture from
/// @return Loaded texture
static unsigned int LoadTexture(const Engine::Image& image)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

Graphics::Texture::Texture(const std::string& imageFilePath, Type type)
    : m_texture(LoadTexture({ imageFilePath, true }))
    , m_type(type)
{
    setFiltering(GL_LINEAR);
}

Graphics::Texture::Texture(const uint8_t* data, size_t length, Type type)
    : m_texture(LoadTexture({ data, length, false }))
    , m_type(type)
{
    setFiltering(GL_LINEAR);
}

Graphics::Texture::Texture(Texture&& other) noexcept
    : m_texture(other.m_texture)
    , m_type(other.m_type)
{
    other.m_texture = 0;
}

Graphics::Texture::~Texture()
{
    if (m_texture)
        glDeleteTextures(1, &m_texture);
}

void Graphics::Texture::setFiltering(int direction, int mode)
{
    bind();
    glTexParameteri(GL_TEXTURE_2D, direction, mode);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Graphics::Texture::setFiltering(int mode)
{
    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Graphics::Texture::setWrapping(int direction, int mode)
{
    bind();
    glTexParameteri(GL_TEXTURE_2D, direction, mode);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Graphics::Texture::setWrapping(int mode)
{
    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
    glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace rf
