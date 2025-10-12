#pragma once

#include <utility>

#include <rf/auxiliary/gl.hpp>

#include <rf/core/image.hpp>

namespace rf {

namespace Graphics {
    class Texture {
    public:
        enum class Type : GLenum {
            Diffuse  = 0,   // Texture that defines the base surface color, used in ambient and diffuse terms of the Phong lighting model
            Specular = 1,   // Texture that defines surface shininess, used in the specular term of the Phong lighting model
        };

        static void Unbind(Type type) {
            glActiveTexture(GL_TEXTURE0 + std::to_underlying(type));
            glBindTexture(GL_TEXTURE_2D, 0);
        }

    private:
        GLuint m_texture = 0;

    public:
        Texture(const Image& image);

        Texture(const Texture& other) = delete;

        Texture(Texture&& other) noexcept;

        ~Texture();

    public:
        Texture& operator=(const Texture& other) = delete;

        Texture& operator=(Texture&& other) noexcept;

    private:
        void free();

    public:
        void setFiltering(int direction, int mode) const;

        void setFiltering(int mode) const;

        void setWrapping(int direction, int mode) const;

        void setWrapping(int mode) const;

    public:
        void bind(Type type) const {
            glActiveTexture(GL_TEXTURE0 + std::to_underlying(type));
            glBindTexture(GL_TEXTURE_2D, m_texture);
        }

        void unbind(Type type) const {
            Unbind(type);
        }

    public:
        // TODO: The only user of this is the UI. Use friends?
        GLuint handle() const {
            return m_texture;
        }
    };
}

} // namespace rf
