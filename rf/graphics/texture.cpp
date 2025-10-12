#include "texture.hpp"

#include <rf/core/image.hpp>

namespace rf {

Graphics::Texture::Texture(const Image& image) {
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.dimensions().width, image.dimensions().height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    setFiltering(GL_LINEAR);
}

Graphics::Texture::Texture(Texture&& other) noexcept
    : m_texture(std::exchange(other.m_texture, 0))
{}

Graphics::Texture::~Texture() {
    free();
}

Graphics::Texture& Graphics::Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        free();
        m_texture = std::exchange(other.m_texture, 0);
    }
    return *this;
}

void Graphics::Texture::free() {
    if (m_texture) {
        glDeleteTextures(1, &m_texture);
        m_texture = 0;
    }
}

void Graphics::Texture::setFiltering(int direction, int mode) const {
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, direction, mode);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Graphics::Texture::setFiltering(int mode) const {
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Graphics::Texture::setWrapping(int direction, int mode) const {
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, direction, mode);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Graphics::Texture::setWrapping(int mode) const {
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
    glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace rf
