#pragma once

#include <utility>

#include <rf/auxiliary/gl.hpp>

namespace rf {

namespace Graphics {
    class Storage {
    public:
        enum class Type: GLuint {
            None = GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS,
            Lighting = 0,
        };

    private:
        GLuint m_storage = 0;
        size_t m_size = 0;
        Type m_type = Type::None;

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
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, std::to_underlying(m_type), m_storage);
        }

        size_t size() const {
            return m_size;
        }
    };
}

} // namespace rf
