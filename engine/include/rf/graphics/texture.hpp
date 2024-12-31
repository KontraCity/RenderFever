#pragma once

// STL modules
#include <memory>
#include <string>

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

        enum class Type
        {
            None,       // No texture
            Diffuse,    // Diffuse texture
            Specular,   // Specular map
        };

    public:
        /// @brief Unbind texture
        static inline void Unbind()
        {
            glBindTexture(GL_TEXTURE_2D, 0);
        }

    private:
        unsigned int m_texture = 0;
        Type m_type = Type::None;

    public:
        /// @brief Load texture from file
        /// @param imageFilePath Path to the image file
        /// @param type Texture type
        /// @throw std::runtime_error if the texture couldn't be loaded
        Texture(const std::string& imageFilePath, Type type);

        /// @brief Load texture from buffer
        /// @param data The buffer to load
        /// @param length Length of the buffer
        /// @param type Texture type
        Texture(const uint8_t* data, size_t length, Type type);

        Texture(const Texture& other) = delete;

        Texture(Texture&& other) noexcept;

        ~Texture();

    public:
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
        /// @brief Bind this texture
        inline void bind() const
        {
            glBindTexture(GL_TEXTURE_2D, m_texture);
        }
    
        /// @brief Get texture type
        /// @return Texture type
        inline Type type() const
        {
            return m_type;
        }

        /// @brief Get texture type
        /// @return Texture type
        inline Type& type()
        {
            return m_type;
        }
    };
}

} // namespace rf
