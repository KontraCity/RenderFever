#pragma once

#include <string>

#include <GL/glew.h>

#include "rf/graphics/camera.hpp"
#include "rf/graphics/mesh.hpp"
#include "rf/graphics/transform.hpp"

namespace rf {

class Shader {
public:
    class Handle {
    public:
        Handle(GLuint shaderProgram) {
            glUseProgram(shaderProgram);
        }

        ~Handle() {
            glUseProgram(0);
        }
    };

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

    void set(const std::string& name, bool boolean) const;

    void set(const std::string& name, int integer) const;

    void set(const std::string& name, float real) const;

    void set(const std::string& name, const glm::vec3& vector) const;

    void set(const std::string& name, const glm::mat4& matrix) const;

public:
    void capture(const Camera& camera) const;

    void transform(const Transform& transform) const;

    void draw(const Mesh& mesh) const;

public:
    Handle handle() const {
        return { m_shaderProgram };
    }
};

} // namespace rf
