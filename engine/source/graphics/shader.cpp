#include "rf/graphics/shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "rf/core/engine.hpp"
#include "rf/core/error.hpp"
#include "rf/core/io.hpp"

namespace rf {

static std::string MakeUniformName(const std::string& name) {
    return 'u' + name;
}

static const char* ShaderName(int type) {
    switch (type) {
        case GL_VERTEX_SHADER:      return "vertex";
        case GL_GEOMETRY_SHADER:    return "geometry";
        case GL_FRAGMENT_SHADER:    return "fragment";
        default:                    return "unknown";
    }
}

static GLuint CompileShader(const char* source, int type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint result = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result)
        return shader;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &result);
    std::string error(result, '\0');
    glGetShaderInfoLog(shader, result, nullptr, error.data());
    throw RF_LOCATED_ERROR("Couldn't compile {} shader", ShaderName(type)).withDetails(error);
}

static GLuint LinkShaderProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    if (geometryShader)
        glAttachShader(program, geometryShader);
    glLinkProgram(program);

    GLint result = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (result)
        return program;

    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &result);
    std::string error(result, '\0');
    glGetProgramInfoLog(program, result, nullptr, error.data());
    throw RF_LOCATED_ERROR("Couldn't link shader program").withDetails(error);
}

Graphics::Shader::Shader(const std::string& vertexSourceFilename, const std::string& fragmentSourceFilename, const std::string& geometrySourceFilename) {
    try {
        std::string source = IO::ReadFile(vertexSourceFilename);
        m_vertexShader = CompileShader(source.c_str(), GL_VERTEX_SHADER);

        source = IO::ReadFile(fragmentSourceFilename);
        m_fragmentShader = CompileShader(source.c_str(), GL_FRAGMENT_SHADER);

        if (!geometrySourceFilename.empty()) {
            source = IO::ReadFile(geometrySourceFilename);
            m_geometryShader = CompileShader(source.c_str(), GL_GEOMETRY_SHADER);
        }
        
        m_shaderProgram = LinkShaderProgram(m_vertexShader, m_fragmentShader, m_geometryShader);
        free(true); // program is ready, compiled shaders are no longer needed
    }
    catch (...) {
        free();
        throw;
    }
}

Graphics::Shader::~Shader() {
    free();
}

void Graphics::Shader::free(bool onlyFreeShaders) {
    if (m_vertexShader) {
        glDeleteShader(m_vertexShader);
        m_vertexShader = 0;
    }

    if (m_fragmentShader) {
        glDeleteShader(m_fragmentShader);
        m_fragmentShader = 0;
    }

    if (m_geometryShader) {
        glDeleteShader(m_geometryShader);
        m_geometryShader = 0;
    }

    if (!onlyFreeShaders && m_shaderProgram) {
        glDeleteProgram(m_shaderProgram);
        m_shaderProgram = 0;
    }
}

void Graphics::Shader::set(const std::string& name, bool boolean) const {
    Handle shaderHandle = handle();
    int location = glGetUniformLocation(m_shaderProgram, MakeUniformName(name).c_str());
    glUniform1i(location, static_cast<int>(boolean));
}

void Graphics::Shader::set(const std::string& name, int integer) const {
    Handle shaderHandle = handle();
    int location = glGetUniformLocation(m_shaderProgram, MakeUniformName(name).c_str());
    glUniform1i(location, integer);
}

void Graphics::Shader::set(const std::string& name, float real) const {
    Handle shaderHandle = handle();
    int location = glGetUniformLocation(m_shaderProgram, MakeUniformName(name).c_str());
    glUniform1f(location, real);
}

void Graphics::Shader::set(const std::string& name, const glm::vec3& vector) const {
    Handle shaderHandle = handle();
    int location = glGetUniformLocation(m_shaderProgram, MakeUniformName(name).c_str());
    glUniform3fv(location, 1, glm::value_ptr(vector));
}

void Graphics::Shader::set(const std::string& name, const glm::mat4& matrix) const {
    Handle shaderHandle = handle();
    int location = glGetUniformLocation(m_shaderProgram, MakeUniformName(name).c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Graphics::Shader::capture(const Camera& camera) const {
    const Window::Dimensions& dimensions = Engine::Window().dimensions();
    set("View", glm::lookAt(camera.position(), camera.position() + camera.evaluateDirection(), CameraConst::Up));
    set("Projection", glm::perspective(glm::radians(45.0f / camera.zoom()), dimensions.evaluateRatio(), CameraConst::NearPlane, CameraConst::FarPlane));
}

void Graphics::Shader::transform(const Transform& transform) const {
    glm::mat4 model = transform.evaluateModel();
    set("Model", model);
}

void Graphics::Shader::draw(const Mesh& mesh) const {
    Handle shaderHandle = handle();
    glBindVertexArray(mesh.vertexArray());
    glDrawElements(GL_TRIANGLES, mesh.indicesCount(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

} // namespace rf
