#pragma once

#include <memory>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;

#include <GL/glew.h>

namespace rf {

class Texture {
public:
    using Pointer = std::shared_ptr<Texture>;

    enum Type : GLenum {
        TextureType = GL_TEXTURE0,
        SpecularType = GL_TEXTURE1,
    };

public:
    static void Unbind(Type type) {
        glActiveTexture(type);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

private:
    GLuint m_texture = 0;

public:
    Texture(const fs::path& filePath);

    Texture(const uint8_t* data, size_t length);

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
        glActiveTexture(type);
        glBindTexture(GL_TEXTURE_2D, m_texture);
    }
};

} // namespace rf
