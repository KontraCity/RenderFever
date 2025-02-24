#pragma once

#include <string>

#include <glm/glm.hpp>

#include "rf/graphics/light/types.hpp"
#include "rf/graphics/color.hpp"
#include "rf/graphics/cubemap.hpp"
#include "rf/graphics/texture.hpp"

namespace rf {

namespace Graphics {
    class Shader {
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
        void free(bool freeShaderProgram = true);

    public:
        void make(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath, const std::string& geometryShaderFilePath = {});

        void use() const;

        void set(const std::string& name, bool boolean);

        void set(const std::string& name, int integer);

        void set(const std::string& name, float real);

        void set(const std::string& name, const glm::vec3& vector);

        void set(const std::string& name, const glm::mat4& matrix);

        void set(const std::string& name, const Light::Properties& properties);

        void set(const std::string& name, const Light::Attenuation& attenuation);

        void set(const std::string& name, Light::Cutoff cutoff);

        void set(const std::string& name, Color color);

        void set(const std::string& name, const Cubemap& cubemap, int id);

        void set(const std::string& name, const Texture& texture, int id);
    };
}

} // namespace rf
