#include "texture.hpp"

namespace rf {

Graphics::Texture::Texture(const Image& image)
    : m_dimensions({
        .width = static_cast<int>(image.dimensions().width),
        .height = static_cast<int>(image.dimensions().height),
      }) {
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA8,
        m_dimensions.width,
        m_dimensions.height,
        0, GL_RGBA, GL_UNSIGNED_BYTE,
        image.data()
    );

    // TODO: Custom filtering and wrapping setters? No idea how to use them now.

    // Filtering
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Graphics::Texture::Texture(GLuint texture, const Dimensions& dimensions)
    : m_texture(texture)
    , m_dimensions(dimensions)
{}

Graphics::Texture::Texture(Texture&& other) noexcept
    : m_texture(std::exchange(other.m_texture, 0))
    , m_dimensions(std::exchange(other.m_dimensions, {}))
{}

Graphics::Texture::~Texture() {
    free();
}

Graphics::Texture& Graphics::Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        free();
        m_texture = std::exchange(other.m_texture, 0);
        m_dimensions = std::exchange(other.m_dimensions, {});
    }
    return *this;
}

void Graphics::Texture::free() {
    if (m_texture) {
        glDeleteTextures(1, &m_texture);
        m_texture = 0;
    }
    m_dimensions = {};
}

} // namespace rf
