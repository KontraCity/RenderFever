#pragma once

#include <string>

#include <rf/auxiliary/gl.hpp>
#include <rf/auxiliary/glm.hpp>

#include <rf/graphics/camera.hpp>
#include <rf/graphics/material.hpp>
#include <rf/graphics/mesh.hpp>
#include <rf/graphics/texture.hpp>
#include <rf/graphics/transform.hpp>
#include <rf/resources/library.hpp>

namespace rf {

namespace Graphics {
    class Shader {
    public:
        struct Config {
            std::string vertexSource;
            std::string geometrySource;
            std::string fragmentSource;
        };

    private:
        GLuint m_vertexShader = 0;
        GLuint m_fragmentShader = 0;
        GLuint m_geometryShader = 0;
        GLuint m_shaderProgram = 0;

    public:
        Shader(const Config& config);

        Shader(const Shader& other) = delete;

        Shader(Shader&& other) noexcept;

        ~Shader();

    public:
        Shader& operator=(const Shader& other) = delete;

        Shader& operator=(Shader&& other) noexcept;

    private:
        void free(bool onlyFreeShaders = false);

        void set(const std::string& name, bool value) const;

        void set(const std::string& name, int value) const;

        void set(const std::string& name, float value) const;

        void set(const std::string& name, const glm::vec3& value) const;

        void set(const std::string& name, const glm::mat4& value) const;

        void set(const std::string& name, const Resources::Texture& value, Texture::Type type) const;

    public:
        void capture(const Camera& camera) const;

        void transform(const Transform& transform) const;

        void material(const Material& material) const;

        void draw(const Mesh& mesh) const;
     
    public:
        void use() const {
            glUseProgram(m_shaderProgram);
        }

        void unuse() const {
            glUseProgram(0);
        }
    };
}

} // namespace rf
