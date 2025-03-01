#pragma once

#include <memory>
#include <string>

#include <GL/glew.h>

#include "rf/core/image.hpp"

namespace rf {

namespace Graphics {
    class Texture {
    public:
        using Pointer = std::shared_ptr<Texture>;

        enum class Type {
            None,       // No texture
            Diffuse,    // Diffuse texture
            Specular,   // Specular map
        };

    public:
        static inline void Unbind() {
            glBindTexture(GL_TEXTURE_2D, 0);
        }

    private:
        unsigned int m_texture = 0;
        Type m_type = Type::None;

    public:
        Texture(const std::string& imageFilePath, Type type);

        Texture(const uint8_t* data, size_t length, Type type);

        Texture(const Texture& other) = delete;

        Texture(Texture&& other) noexcept;

        ~Texture();

    public:
        void setFiltering(int direction, int mode);

        void setFiltering(int mode);

        void setWrapping(int direction, int mode);

        void setWrapping(int mode);

    public:
        inline void bind() const {
            glBindTexture(GL_TEXTURE_2D, m_texture);
        }
    
        inline Type type() const {
            return m_type;
        }

        inline Type& type() {
            return m_type;
        }
    };
}

} // namespace rf
