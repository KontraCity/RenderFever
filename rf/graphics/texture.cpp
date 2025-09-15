#include "texture.hpp"

#include "rf/core/image.hpp"

namespace rf {

static GLuint LoadTexture(const Image& image) {
    GLuint texture = 0;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

Texture::Texture(const std::string& filename, Type type)
    : m_texture(LoadTexture({ filename, true }))
    , m_type(type) {
    setFiltering(GL_LINEAR);
}

Texture::Texture(const uint8_t* data, size_t length, Type type)
    : m_texture(LoadTexture({ data, length, false }))
    , m_type(type) {
    setFiltering(GL_LINEAR);
}

Texture::Texture(Texture&& other) noexcept
    : m_texture(other.m_texture)
    , m_type(other.m_type) {
    other.m_texture = 0;
    other.m_type = Type::None;
}

Texture::~Texture() {
    free();
}

Texture& Texture::operator=(Texture&& other) noexcept {
    free();

    m_texture = other.m_texture;
    m_type = other.m_type;

    other.m_texture = 0;
    other.m_type = Type::None;

    return *this;
}

void Texture::free() {
    if (m_texture) {
        glDeleteTextures(1, &m_texture);
        m_texture = 0;
    }
    m_type = Type::None;
}

void Texture::setFiltering(int direction, int mode) {
    bind();
    glTexParameteri(GL_TEXTURE_2D, direction, mode);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setFiltering(int mode) {
    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setWrapping(int direction, int mode) {
    bind();
    glTexParameteri(GL_TEXTURE_2D, direction, mode);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setWrapping(int mode) {
    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
    glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace rf
