#pragma once

// STL modules
#include <string>

// Graphics libraries
#include <glm/glm.hpp>

// Custom modules
#include "rf/graphics/light/types.hpp"
#include "rf/graphics/color.hpp"
#include "rf/graphics/cubemap.hpp"
#include "rf/graphics/texture.hpp"

namespace rf {

namespace Graphics
{
    class Shader
    {
    private:
        unsigned int m_vertexShader = 0;
        unsigned int m_geometryShader = 0;
        unsigned int m_fragmentShader = 0;
        unsigned int m_shaderProgram = 0;

    public:
        Shader() = default;

        Shader(const Shader& other) = delete;

        Shader(Shader&& other) noexcept = delete;

        ~Shader();

    private:
        /// @brief Free allocated resources
        /// @param freeShaderProgram Whether to free shader program or not
        void free(bool freeShaderProgram = true);

    public:
        /// @brief Read, compile shaders and link shader program
        /// @param vertexShaderFilePath Path to vertex shader source file
        /// @param fragmentShaderFilePath Path to fragment shader source file
        /// @param geometryShaderFilePath Path to geometry shader source file
        /// @throw std::runtime_error if read/compile/link error occurs
        void make(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath, const std::string& geometryShaderFilePath = {});

        /// @brief Tell OpenGL to use this shader
        void use() const;

        /// @brief Set uniform boolean
        /// @param name Boolean name
        /// @param boolean The boolean to set
        void set(const std::string& name, bool boolean);

        /// @brief Set uniform integer
        /// @param name Integer name
        /// @param integer The integer to set
        void set(const std::string& name, int integer);

        /// @brief Set uniform real
        /// @param name Real name
        /// @param real The real to set
        void set(const std::string& name, float real);

        /// @brief Set uniform vector
        /// @param name Vector name
        /// @param vector The vector to set
        void set(const std::string& name, const glm::vec3& vector);

        /// @brief Set uniform matrix
        /// @param name Matrix name
        /// @param matrix The matrix to set
        void set(const std::string& name, const glm::mat4& matrix);

        /// @brief Set uniform light properties
        /// @param name Light properties name
        /// @param properties The light properties to set
        void set(const std::string& name, const Light::Properties& properties);

        /// @brief Set uniform light attenuation
        /// @param name Light attenuation name
        /// @param attenuation The light attenuation to set
        void set(const std::string& name, const Light::Attenuation& attenuation);

        /// @brief Set uniform light cutoff
        /// @param name Light cutoff name
        /// @param cutoff The light cutoff to set
        void set(const std::string& name, Light::Cutoff cutoff);

        /// @brief Set uniform color
        /// @param name Color name
        /// @param color The color to set
        void set(const std::string& name, Color color);

        /// @brief Set uniform cubemap
        /// @param name Cubemap name
        /// @param cubemap The cubemap to set
        /// @param id Cubemap ID
        void set(const std::string& name, const Cubemap& cubemap, int id);

        /// @brief Set uniform texture
        /// @param name Texture name
        /// @param texture The texture to set
        /// @param id Texture ID
        void set(const std::string& name, const Texture& texture, int id);
    };
}

} // namespace rf
