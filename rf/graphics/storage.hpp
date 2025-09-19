#pragma once

#include <GL/glew.h>

namespace rf {

class Storage {
public:
    enum Type : GLuint {
        NoneType = GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS,
        LightingType = 0,
    };

private:
    GLuint m_storage = 0;
    size_t m_size = 0;
    Type m_type = NoneType;

public:
    Storage(Type type, size_t size = 0, const void* data = nullptr);

    Storage(const Storage& other) = delete;

    Storage(Storage&& other) noexcept;

    ~Storage();

public:
    Storage& operator=(const Storage& other) = delete;

    Storage& operator=(Storage&& other) noexcept;

private:
    void free();

public:
    void resize(size_t newSize);

    size_t write(const void* data, size_t size, size_t offset = 0);

public:
    void bind() const {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_type, m_storage);
    }

    size_t size() const {
        return m_size;
    }
};

} // namespace rf
