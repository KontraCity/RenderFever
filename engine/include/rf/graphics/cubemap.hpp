#pragma once

// STL modules
#include <memory>
#include <string>

namespace rf {
    
namespace Graphics
{
    class Cubemap
    {
    public:
        // Shared cubemap instance
        using Pointer = std::shared_ptr<Cubemap>;

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
