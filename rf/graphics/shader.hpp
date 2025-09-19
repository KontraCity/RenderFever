#pragma once

#include <string>
#include <filesystem>
namespace fs = std::filesystem;

#include <GL/glew.h>

#include "rf/core/assets.hpp"
#include "rf/graphics/camera.hpp"
#include "rf/graphics/lighting.hpp"
#include "rf/graphics/material.hpp"
#include "rf/graphics/mesh.hpp"
#include "rf/graphics/texture.hpp"
#include "rf/graphics/transform.hpp"

namespace rf {

class Shader {
private:
    GLuint m_vertexShader = 0;
    GLuint m_fragmentShader = 0;
    GLuint m_geometryShader = 0;
    GLuint m_shaderProgram = 0;
    std::string m_name;

public:
    Shader(const fs::path& directoryPath);

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

    void set(const std::string& name, const Assets::Texture& value, Texture::Type type) const;

public:
    void capture(const Camera& camera) const;

    void transform(const Transform& transform) const;

    void material(const Material& material) const;

    void illuminate(const Light& light) const;

    void draw(const Mesh& mesh) const;
     
public:
    void use() const {
        glUseProgram(m_shaderProgram);
    }
};

} // namespace rf
