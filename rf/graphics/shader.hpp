#pragma once

#include <string>

#include <GL/glew.h>

#include "rf/graphics/camera.hpp"
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

public:
    Shader(const std::string& vertexSourceFilename, const std::string& fragmentSourceFilename, const std::string& geometrySourceFilename = {});

    Shader(const Shader& other) = delete;

    Shader(Shader&& other) = delete;

    ~Shader();

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

    void draw(const Mesh& mesh) const;
     
public:
    void use() const {
        glUseProgram(m_shaderProgram);
    }
};

} // namespace rf
