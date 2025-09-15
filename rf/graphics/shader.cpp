#include "shader.hpp"

#include <vector>
#include <regex>
#include <algorithm>
#include <filesystem>
namespace fs = std::filesystem;

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "rf/core/engine.hpp"
#include "rf/core/error.hpp"
#include "rf/core/io.hpp"

namespace rf {

static std::string ReadSource(fs::path path, std::vector<fs::path>& included) {
    path = fs::canonical(path);
    included.push_back(fs::canonical(path));

    std::string source = IO::ReadFile(path.string());
    while (true) {
        std::smatch matches;
        std::regex regex(R"(^[ \t]*#include[ \t]*["<](.+)[">][ \t]*$)");
        if (!std::regex_search(source, matches, regex))
            return source;

        std::string includeSource;
        fs::path includePath = fs::canonical(path.parent_path() / matches.str(1));
        if (std::find(included.begin(), included.end(), includePath) == included.end())
            includeSource = ReadSource(path.parent_path() / matches.str(1), included);
        source.replace(matches.position(), matches.length(), includeSource);
    }
}

static std::string ReadSource(const fs::path& path) {
    std::vector<fs::path> included;
    return ReadSource(path, included);
}

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

Shader::Shader(const Config& config) {
    try {
        std::string source = ReadSource(config.vertexSourceFilename);
        m_vertexShader = CompileShader(source.c_str(), GL_VERTEX_SHADER);

        source = ReadSource(config.fragmentSourceFilename);
        m_fragmentShader = CompileShader(source.c_str(), GL_FRAGMENT_SHADER);

        if (!config.geometrySourceFilename.empty()) {
            source = ReadSource(config.geometrySourceFilename);
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

Shader::Shader(Shader&& other) noexcept
    : m_vertexShader(0)
    , m_geometryShader(0)
    , m_fragmentShader(0)
    , m_shaderProgram(0) {
    other.m_vertexShader = 0;
    other.m_geometryShader = 0;
    other.m_fragmentShader = 0;
    other.m_shaderProgram = 0;
}

Shader::~Shader() {
    free();
}

Shader& Shader::operator=(Shader&& other) noexcept {
    free();

    m_vertexShader = other.m_vertexShader;
    m_geometryShader = other.m_geometryShader;
    m_fragmentShader = other.m_fragmentShader;
    m_shaderProgram = other.m_shaderProgram;

    other.m_vertexShader = 0;
    other.m_geometryShader = 0;
    other.m_fragmentShader = 0;
    other.m_shaderProgram = 0;

    return *this;
}

void Shader::free(bool onlyFreeShaders) {
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

void Shader::set(const std::string& name, bool boolean) {
    use();
    int location = glGetUniformLocation(m_shaderProgram, MakeUniformName(name).c_str());
    glUniform1i(location, static_cast<int>(boolean));
}

void Shader::set(const std::string& name, int integer) {
    use();
    int location = glGetUniformLocation(m_shaderProgram, MakeUniformName(name).c_str());
    glUniform1i(location, integer);
}

void Shader::set(const std::string& name, float real) {
    use();
    int location = glGetUniformLocation(m_shaderProgram, MakeUniformName(name).c_str());
    glUniform1f(location, real);
}

void Shader::set(const std::string& name, const glm::vec3& vector) {
    use();
    int location = glGetUniformLocation(m_shaderProgram, MakeUniformName(name).c_str());
    glUniform3fv(location, 1, glm::value_ptr(vector));
}

void Shader::set(const std::string& name, const glm::mat4& matrix) {
    use();
    int location = glGetUniformLocation(m_shaderProgram, MakeUniformName(name).c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::set(const std::string& name, const Texture& texture, int id) {
    texture.bind(id);
    set(name, id);
}

void Shader::capture(const Camera& camera) {
    glm::mat4 view = camera.evaluateView();
    set("View", view);

    float aspectRatio = Engine::Window().getDimensions().evaluateRatio();
    glm::mat4 projection = camera.evaluateProjection(aspectRatio);
    set("Projection", projection);
}

void Shader::transform(const Transform& transform) {
    glm::mat4 model = transform.evaluateModel();
    set("Model", model);
}

void Shader::material(const Material& material) {
    if (material.texture)
        set("Material.texture", *material.texture, 0);
    if (material.specular)
        set("Material.specular", *material.specular, 1);
    set("Material.shininess", material.shininess);
}

void Shader::illuminate(const Light& light) {
    set("Light.position", light.position);
    set("Light.direction", light.direction);
    set("Light.color", light.color);

    set("Light.type", static_cast<int32_t>(light.type));
    set("Light.ambientProperty", light.ambientProperty);
    set("Light.diffuseProperty", light.diffuseProperty);
    set("Light.specularProperty", light.specularProperty);
    
    set("Light.constantAttenuation", light.constantAttenuation);
    set("Light.linearAttenuation", light.linearAttenuation);
    set("Light.quadraticAttenuation", light.quadraticAttenuation);

    set("Light.spotInnerCutoff", light.spotInnerCutoff);
    set("Light.spotOuterCutoff", light.spotOuterCutoff);
}

void Shader::draw(const Mesh& mesh) const {
    if (mesh) {
        use();
        glBindVertexArray(mesh.vertexArray());
        glDrawElements(GL_TRIANGLES, mesh.indicesCount(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

} // namespace rf
