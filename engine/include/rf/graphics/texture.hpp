#pragma once

// STL modules
#include <memory>
#include <string>
#include <stdexcept>

// Graphics libraries
#include <GL/glew.h>

// Custom modules
#include "rf/engine/image.hpp"

namespace rf {

namespace Graphics
{
    class Texture
    {
    public:
        // Shared texture instance
        using Pointer = std::shared_ptr<Texture>;

    private:
        /// @brief Load texture from image file
        /// @param imageFilePath Path to the image file
        /// @param format Image format (GL_RGB, GL_RGBA, etc)
        /// @throw std::runtime_error if the texture couldn't be loaded
        static unsigned int LoadTexture(const std::string& imageFilePath, int format);

    private:
        unsigned int m_texture = 0;

    public:
        Texture() noexcept = default;

        /// @brief Load texture from image file
        /// @param imageFilePath Path to the image file
        /// @param format Image format (GL_RGB, GL_RGBA, etc)
        /// @throw std::runtime_error if the texture couldn't be loaded
        Texture(const std::string& imageFilePath, int format = GL_RGB);

        Texture(const Texture& other) = delete;

        Texture(Texture&& other) noexcept;

        ~Texture();

    private:
        /// @brief Free allocated resources
        void free();

    public:
        /// @brief Load texture from image file
        /// @param imageFilePath Path to the image file
        /// @param format Image format (GL_RGB, GL_RGBA, etc)
        /// @throw std::runtime_error if the texture couldn't be loaded
        void load(const std::string& imageFilePath, int format = GL_RGB);

        /// @brief Bind this texture
        void bind() const;

        /// @brief Set texture filtering mode for direction
        /// @param direction Filtering direction (GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER, etc)
        /// @param mode The filtering mode to set (GL_NEAREST, GL_LINEAR, etc)
        void setFiltering(int direction, int mode);

        /// @brief Set texture filtering mode for all directions
        /// @param mode The filtering mode to set (GL_NEAREST, GL_LINEAR, etc)
        void setFiltering(int mode);

        /// @brief Set texture wrapping mode for direction
        /// @param direction Wrapping direction (GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, etc)
        /// @param mode The wrapping mode to set (GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER, etc)
        void setWrapping(int direction, int mode);

        /// @brief Set texture wrapping mode for all directions
        /// @param mode The wrapping mode to set (GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER, etc)
        void setWrapping(int mode);

    public:
        /// @brief Check if texture is loaded
        /// @return True if texture is loaded
        inline operator bool() const
        {
            return static_cast<bool>(m_texture);
        }
    };
}

} // namespace rf
