#pragma once

#include <memory>
#include <string>

#include <GL/glew.h>

namespace rf {

class Texture {
public:
    using Pointer = std::shared_ptr<Texture>;

    enum class Type {
        None,       // No texture
        Texture,    // Base texture (Diffuse)
        Specular,   // Specular map
    };

private:
    GLuint m_texture = 0;
    Type m_type = Type::None;

public:
    Texture(const std::string& filename, Type type);

    Texture(const uint8_t* data, size_t length, Type type);

    Texture(const Texture& other) = delete;

    Texture(Texture&& other) noexcept;

    ~Texture();

public:
    Texture& operator=(const Texture& other) = delete;

    Texture& operator=(Texture&& other) noexcept;

private:
    void reset();

public:
    void setFiltering(int direction, int mode);

    void setFiltering(int mode);

    void setWrapping(int direction, int mode);

    void setWrapping(int mode);

public:
    void bind(int id = 0) const {
        glActiveTexture(GL_TEXTURE0 + id);
        glBindTexture(GL_TEXTURE_2D, m_texture);
    }

    Type type() const {
        return m_type;
    }

    Type& type() {
        return m_type;
    }
};

} // namespace rf
