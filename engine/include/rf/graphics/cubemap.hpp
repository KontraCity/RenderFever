#pragma once

#include <memory>
#include <string>

#include <GL/glew.h>

namespace rf {
    
namespace Graphics {
    class Cubemap {
    public:
        using Pointer = std::shared_ptr<Cubemap>;

    public:
        static inline void Unbind() {
            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        }

    private:
        unsigned int m_cubemap = 0;

    public:
        Cubemap(const std::string& cubemapDirectoryPath);

        Cubemap(const Cubemap& other) = delete;

        Cubemap(Cubemap&& other) noexcept;

        ~Cubemap();

    public:
        inline void bind() const {
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemap);
        }
    };
}

} // namespace rf
