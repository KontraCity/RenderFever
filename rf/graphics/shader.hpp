#pragma once

#include <string>

#include <GL/glew.h>

#include "rf/graphics/camera.hpp"
#include "rf/graphics/lighting.hpp"
#include "rf/graphics/material.hpp"
#include "rf/graphics/mesh.hpp"
#include "rf/graphics/texture.hpp"
#include "rf/graphics/transform.hpp"

namespace rf {

class Shader {
public:
    enum class Type {
        Main,
        Light,
    };

public:
    struct Config {
        std::string vertexSourceFilename;
        std::string geometrySourceFilename;
        std::string fragmentSourceFilename;
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

    void set(const std::string& name, bool boolean);

    void set(const std::string& name, int integer);

    void set(const std::string& name, float real);

    void set(const std::string& name, const glm::vec3& vector);

    void set(const std::string& name, const glm::mat4& matrix);

    void set(const std::string& name, const Texture& texture, int id = 0);

public:
    void capture(const Camera& camera);

    void transform(const Transform& transform);

    void material(const Material& material);

    void illuminate(const Light& light);

    void draw(const Mesh& mesh) const;
     
public:
    void use() const {
        glUseProgram(m_shaderProgram);
    }
};

} // namespace rf
