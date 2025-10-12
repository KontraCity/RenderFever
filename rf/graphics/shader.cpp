#include "shader.hpp"

#include <utility>

#include <rf/core/error.hpp>
#include <rf/core/math.hpp>

namespace rf {

static constexpr const char* ShaderName(int type) {
    switch (type) {
        case GL_VERTEX_SHADER:      return "vertex";
        case GL_GEOMETRY_SHADER:    return "geometry";
        case GL_FRAGMENT_SHADER:    return "fragment";
        default:                    return "unknown";
    }
}

static GLuint CompileShader(const std::string& source, GLenum type) {
    GLuint shader = glCreateShader(type);
    const char* data = source.c_str();
    glShaderSource(shader, 1, &data, nullptr);
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

static std::string MakeUniformName(const std::string& name) {
    return 'u' + name;
}

static Graphics::Dimensions GetViewportDimensions() {
    GLint parameters[4];
    glGetIntegerv(GL_VIEWPORT, parameters);

    // parameters[0] = x
    // parameters[1] = y
    // parameters[2] = width
    // parameters[3] = height
    return { parameters[2], parameters[3] };
}

Graphics::Shader::Shader(const Config& config) {
    try {
        m_vertexShader = CompileShader(config.vertexSource, GL_VERTEX_SHADER);
        m_geometryShader = config.geometrySource.empty() ? 0 : CompileShader(config.geometrySource, GL_GEOMETRY_SHADER);
        m_fragmentShader = CompileShader(config.fragmentSource, GL_FRAGMENT_SHADER);
        m_shaderProgram = LinkShaderProgram(m_vertexShader, m_fragmentShader, m_geometryShader);
        free(true); // program is ready, compiled shaders are no longer needed
    }
    catch (...) {
        free();
        throw;
    }
}

Graphics::Shader::Shader(Shader&& other) noexcept
    : m_vertexShader(std::exchange(other.m_vertexShader, 0))
    , m_geometryShader(std::exchange(other.m_geometryShader, 0))
    , m_fragmentShader(std::exchange(other.m_fragmentShader, 0))
    , m_shaderProgram(std::exchange(other.m_shaderProgram, 0))
{}

Graphics::Shader::~Shader() {
    free();
}

Graphics::Shader& Graphics::Shader::operator=(Shader&& other) noexcept {
    if (this != &other) {
        free();
        m_vertexShader = std::exchange(other.m_vertexShader, 0);
        m_geometryShader = std::exchange(other.m_geometryShader, 0);
        m_fragmentShader = std::exchange(other.m_fragmentShader, 0);
        m_shaderProgram = std::exchange(other.m_shaderProgram, 0);
    }
    return *this;
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

void Graphics::Shader::set(const std::string& name, bool value) const {
    use();
    int location = glGetUniformLocation(m_shaderProgram, MakeUniformName(name).c_str());
    glUniform1i(location, static_cast<int>(value));
}

void Graphics::Shader::set(const std::string& name, int value) const {
    use();
    int location = glGetUniformLocation(m_shaderProgram, MakeUniformName(name).c_str());
    glUniform1i(location, value);
}

void Graphics::Shader::set(const std::string& name, float value) const {
    use();
    int location = glGetUniformLocation(m_shaderProgram, MakeUniformName(name).c_str());
    glUniform1f(location, value);
}

void Graphics::Shader::set(const std::string& name, const glm::vec3& value) const {
    use();
    int location = glGetUniformLocation(m_shaderProgram, MakeUniformName(name).c_str());
    glUniform3fv(location, 1, glm::value_ptr(value));
}

void Graphics::Shader::set(const std::string& name, const glm::mat4& value) const {
    use();
    int location = glGetUniformLocation(m_shaderProgram, MakeUniformName(name).c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Graphics::Shader::set(const std::string& name, const Resources::Texture& value, Texture::Type type) const {
    value ? value->bind(type) : Texture::Unbind(type);
    set(name, static_cast<int>(type));
}

void Graphics::Shader::capture(const Camera& camera) const {
    set("View", Math::EvaluateView(camera));
    set("Projection", Math::EvaluateProjection(camera, GetViewportDimensions()));
}

void Graphics::Shader::transform(const Transform& transform) const {
    set("Model", Math::EvaluateModel(transform));
}

void Graphics::Shader::material(const Material& material) const {
    set("Material.complete", material.diffuse.isValid());
    set("Material.diffuse", material.diffuse, Texture::Type::Diffuse);
    set("Material.specular", material.specular, Texture::Type::Specular);
    set("Material.shininess", material.shininess);
}

void Graphics::Shader::draw(const Mesh& mesh) const {
    if (!mesh)
        return;

    use();
    glBindVertexArray(mesh.vertexArray());
    glDrawElements(GL_TRIANGLES, mesh.indicesCount(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

} // namespace rf
