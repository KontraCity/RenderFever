#pragma once

// STL modules
#include <memory>
#include <string>

// Graphics libraries
#include <GL/glew.h>

namespace rf {
    
namespace Graphics
{
    class Cubemap
    {
    public:
        // Shared cubemap instance
        using Pointer = std::shared_ptr<Cubemap>;

    public:
        /// @brief Unbind cubemap
        static inline void Unbind()
        {
            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        }

    private:
        unsigned int m_cubemap = 0;

    public:
        /// @brief Load cubemap from directory
        /// @param cubemapDirectoryPath Path to the cubemap directory
        /// @throw std::runtime_error if the cubemap couldn't be loaded
        Cubemap(const std::string& cubemapDirectoryPath);

        Cubemap(const Cubemap& other) = delete;

        Cubemap(Cubemap&& other) noexcept;

        ~Cubemap();

    public:
        /// @brief Bind this cubemap
        inline void bind() const
        {
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemap);
        }
    };
}

} // namespace rf
