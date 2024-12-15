#pragma once

// STL modules
#include <memory>
#include <string>
#include <map>
#include <filesystem>
#include <stdexcept>

// Graphics libraries
#include <GL/glew.h>

// Custom modules
#include "rf/engine/image.hpp"
#include "rf/engine/utility.hpp"

namespace rf {

/* Namespace aliases and imports */
namespace fs = std::filesystem;
    
namespace Graphics
{
    class Cubemap
    {
    public:
        // Shared cubemap instance
        using Pointer = std::shared_ptr<Cubemap>;

    private:
        /// @brief Load cubemap from directory
        /// @param cubemapDirectoryPath Path to the cubemap directory
        /// @throw std::runtime_error if the cubemap couldn't be loaded
        /// @return Loaded cubemap
        static unsigned int LoadCubemap(const std::string& cubemapDirectoryPath);

    private:
        unsigned int m_cubemap = 0;

    public:
        /// @brief Load cubemap from directory
        /// @param cubemapDirectoryPath Path to the cubemap directory
        /// @throw std::runtime_error if the cubemap couldn't be loaded
        Cubemap(const std::string& cubemapDirectoryPath);

        Cubemap() = default;

        Cubemap(const Cubemap& other) = delete;

        Cubemap(Cubemap&& other) noexcept;

        ~Cubemap();

    private:
        /// @brief Free allocated resources
        void free();

    public:
        /// @brief Load cubemap from directory
        /// @param cubemapDirectoryPath Path to the cubemap directory
        /// @throw std::runtime_error if the cubemap couldn't be loaded
        void load(const std::string& cubemapDirectoryPath);

        /// @brief Bind this cubemap
        void bind() const;

    public:
        /// @brief Check if the cubemap is loaded
        /// @return True if the cubemap is loaded
        inline operator bool() const
        {
            return static_cast<bool>(m_cubemap);
        }
    };
}

} // namespace rf
