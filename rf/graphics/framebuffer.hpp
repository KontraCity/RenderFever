#pragma once

#include <rf/auxiliary/gl.hpp>

#include <rf/graphics/dimensions.hpp>

namespace rf {

namespace Graphics {
    class Framebuffer {
    private:
        GLuint m_framebuffer = 0;
        GLuint m_texture = 0;
        GLuint m_renderbuffer = 0;
        Dimensions m_dimensions = {};

    public:
        Framebuffer(Dimensions dimensions);

        Framebuffer(const Framebuffer& other) = delete;

        Framebuffer(Framebuffer&& other) noexcept;

        ~Framebuffer();

    public:
        Framebuffer& operator=(const Framebuffer& other) = delete;
        
        Framebuffer& operator=(Framebuffer&& other) noexcept;

    private:
        void free();

    public:
        void bind() const;

        void unbind() const;

    public:
        GLuint texture() const {
            return m_texture;
        }
    };
}

} // namespace rf
