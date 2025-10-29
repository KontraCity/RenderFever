#include "framebuffer.hpp"

#include <utility>

#include <rf/core/engine.hpp>
#include <rf/core/error.hpp>

namespace rf {

Graphics::Framebuffer::Framebuffer(Dimensions dimensions) 
    : m_dimensions(dimensions) {
    glGenFramebuffers(1, &m_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, dimensions.width, dimensions.height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);

    glGenRenderbuffers(1, &m_renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, dimensions.width, dimensions.height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderbuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw RF_LOCATED_ERROR("Framebuffer is not complete");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Graphics::Framebuffer::Framebuffer(Framebuffer&& other) noexcept
    : m_framebuffer(std::exchange(other.m_framebuffer, 0))
    , m_texture(std::exchange(other.m_texture, 0))
    , m_renderbuffer(std::exchange(other.m_renderbuffer, 0))
    , m_dimensions(std::exchange(other.m_dimensions, {}))
{}

Graphics::Framebuffer::~Framebuffer() {
    free();
}
   
Graphics::Framebuffer& Graphics::Framebuffer::operator=(Framebuffer&& other) noexcept {
    if (this != &other) {
        free();
        m_framebuffer = std::exchange(other.m_framebuffer, 0);
        m_texture = std::exchange(other.m_texture, 0);
        m_renderbuffer = std::exchange(other.m_renderbuffer, 0);
        m_dimensions = std::exchange(other.m_dimensions, {});
    }
    return *this;
}

void Graphics::Framebuffer::free() {
    if (m_framebuffer) {
        glDeleteFramebuffers(1, &m_framebuffer);
        m_framebuffer = 0;
    }

    if (m_texture) {
        glDeleteTextures(1, &m_texture);
        m_texture = 0;
    }

    if (m_renderbuffer) {
        glDeleteRenderbuffers(1, &m_renderbuffer);
        m_renderbuffer = 0;
    }

    m_dimensions = {};
}

void Graphics::Framebuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    glViewport(0, 0, m_dimensions.width, m_dimensions.height);
}

void Graphics::Framebuffer::unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    Dimensions windowDimensions = Engine::Window().getDimensions();
    glViewport(0, 0, windowDimensions.width, windowDimensions.height);
}

Graphics::Texture Graphics::Framebuffer::texture() const {
    GLuint texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, m_dimensions.width, m_dimensions.height);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint invertedFramebuffer = 0;
    glGenFramebuffers(1, &invertedFramebuffer);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_framebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, invertedFramebuffer);
    
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    glBlitFramebuffer(
        0, 0, m_dimensions.width, m_dimensions.height,
        0, m_dimensions.height, m_dimensions.width, 0,
        GL_COLOR_BUFFER_BIT, GL_NEAREST
    );

    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &invertedFramebuffer);

    return { texture, m_dimensions };
}

} // namespace rf
