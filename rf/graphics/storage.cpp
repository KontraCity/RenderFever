#include "storage.hpp"

namespace rf {

Storage::Storage(GLuint index, size_t size, const void* data)
    : m_index(index)
    , m_size(size) {
    glGenBuffers(1, &m_storage);
    if (!size)
        return;

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_storage);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_index, m_storage);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

Storage::Storage(Storage&& other) noexcept
    : m_storage(other.m_storage)
    , m_size(other.m_size) {
    other.m_storage = 0;
    other.m_size = 0;
}

Storage::~Storage() {
    free();
}

Storage& Storage::operator=(Storage&& other) noexcept {
    free();

    m_storage = other.m_storage;
    m_size = other.m_size;
    
    other.m_storage = 0;
    other.m_size = 0;

    return *this;
}

void Storage::free() {  
    if (m_storage) {
        glDeleteBuffers(1, &m_storage);
        m_storage = 0;
    }
    m_size = 0;
}

void Storage::resize(size_t newSize) {
    if (newSize == m_size)
        return;

    GLuint newStorage = 0;
    glGenBuffers(1, &newStorage);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, newStorage);
    glBufferData(GL_SHADER_STORAGE_BUFFER, newSize, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_index, newStorage);

    glBindBuffer(GL_COPY_READ_BUFFER, m_storage);
    glBindBuffer(GL_COPY_WRITE_BUFFER, newStorage);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, newSize < m_size ? newSize : m_size);
    glDeleteBuffers(1, &m_storage);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    m_storage = newStorage;
    m_size = newSize;
}

size_t Storage::write(const void* data, size_t size, size_t offset) {
    if (offset + size > m_size) 
        resize(offset + size);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_storage);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, data);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    return size;
}

} // namespace rf
