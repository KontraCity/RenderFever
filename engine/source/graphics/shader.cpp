#include "rf/graphics/shader.hpp"

// STL modules
#include <fstream>
#include <sstream>
#include <stdexcept>

// Graphics libraries
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

// Library {fmt}
#include <fmt/format.h>

namespace rf {

/// @brief Read file from disk
/// @param filePath Path to the file
/// @throw std::runtime_error if the file couldn't be opened
/// @return File contents
static std::string ReadFile(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file)
        throw std::runtime_error(fmt::format("rf::Graphics::Shader::ReadFile(): Couldn't open file \"{}\"", filePath));

    std::stringstream stream;
    stream << file.rdbuf();
    return stream.str();
}

/// @brief Convert shader type to name
/// @param type Shader type
/// @return Converted shader name
static const char* TypeToName(int type)
{
    switch (type)
    {
        case GL_VERTEX_SHADER:
            return "vertex";
        case GL_GEOMETRY_SHADER:
            return "geometry";
        case GL_FRAGMENT_SHADER:
            return "fragment";
        default:
            return "unknown";
    }
}

/// @brief Compile shader from source
/// @param source Shader source
/// @param type Shader type
/// @throw std::runtime_error if compile error occurs
/// @return Compiled shader
static unsigned int CompileShader(const char* source, int type)
{
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int result = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result)
        return shader;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &result);
    std::string error(result, '\0');
    glGetShaderInfoLog(shader, result, nullptr, error.data());
    throw std::runtime_error(fmt::format("rf::Graphics::Shader::CompileShader(): Couldn't compile {} shader:\n{}", TypeToName(type), error));
}

/// @brief Link shader program
/// @param vertexShader Compiled vertex shader
/// @param fragmentShader Compiled fragment shader
/// @param geometryShader Compiled geometry shader
/// @throw std::runtime_error if link error occurs
/// @return Linked shader program
static unsigned int LinkShaderProgram(unsigned int vertexShader, unsigned int fragmentShader, unsigned int geometryShader)
{
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    if (geometryShader)
        glAttachShader(program, geometryShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int result = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (result)
        return program;

    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &result);
    std::string error(result, '\0');
    glGetProgramInfoLog(program, result, nullptr, error.data());
    throw std::runtime_error(fmt::format("rf::Graphics::Shader::LinkShaderProgram(): Couldn't link shader program:\n{}", error));
}

Graphics::Shader::~Shader()
{
    free();
}

void Graphics::Shader::free(bool freeShaderProgram)
{
    if (m_vertexShader)
    {
        glDeleteShader(m_vertexShader);
        m_vertexShader = 0;
    }

    if (m_geometryShader)
    {
        glDeleteShader(m_geometryShader);
        m_geometryShader = 0;
    }

    if (m_fragmentShader)
    {
        glDeleteShader(m_fragmentShader);
        m_fragmentShader = 0;
    }

    if (freeShaderProgram && m_shaderProgram)
    {
        glDeleteProgram(m_shaderProgram);
        m_shaderProgram = 0;
    }
}

void Graphics::Shader::make(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath, const std::string& geometryShaderFilePath)
{
    free(); // avoid memory leaks if make() was called already
    m_vertexShader = CompileShader(ReadFile(vertexShaderFilePath).c_str(), GL_VERTEX_SHADER);
    if (!geometryShaderFilePath.empty())
        m_geometryShader = CompileShader(ReadFile(geometryShaderFilePath).c_str(), GL_GEOMETRY_SHADER);
    m_fragmentShader = CompileShader(ReadFile(fragmentShaderFilePath).c_str(), GL_FRAGMENT_SHADER);
    m_shaderProgram = LinkShaderProgram(m_vertexShader, m_fragmentShader, m_geometryShader);
    free(false);
}

void Graphics::Shader::use() const
{
    glUseProgram(m_shaderProgram);
}

void Graphics::Shader::set(const std::string& name, bool boolean)
{
    use();
    int location = glGetUniformLocation(m_shaderProgram, ('u' + name).c_str());
    glUniform1i(location, static_cast<int>(boolean));
}

void Graphics::Shader::set(const std::string& name, int integer)
{
    use();
    int location = glGetUniformLocation(m_shaderProgram, ('u' + name).c_str());
    glUniform1i(location, integer);
}

void Graphics::Shader::set(const std::string& name, float real)
{
    use();
    int location = glGetUniformLocation(m_shaderProgram, ('u' + name).c_str());
    glUniform1f(location, real);
}

void Graphics::Shader::set(const std::string& name, const glm::vec3& vector)
{
    use();
    int location = glGetUniformLocation(m_shaderProgram, ('u' + name).c_str());
    glUniform3fv(location, 1, glm::value_ptr(vector));
}

void Graphics::Shader::set(const std::string& name, const glm::mat4& matrix)
{
    use();
    int location = glGetUniformLocation(m_shaderProgram, ('u' + name).c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Graphics::Shader::set(const std::string& name, const Lighting::Properties& properties)
{
    set(name + ".ambient", properties.ambient);
    set(name + ".diffuse", properties.diffuse);
    set(name + ".specular", properties.specular);
}

void Graphics::Shader::set(const std::string& name, const Lighting::Attenuation& attenuation)
{
    set(name + ".constant", attenuation.constant);
    set(name + ".linear", attenuation.linear);
    set(name + ".quadratic", attenuation.quadratic);
}

void Graphics::Shader::set(const std::string& name, Lighting::Cutoff cutoff)
{
    set(name + ".inner", glm::cos(glm::radians(cutoff.inner)));
    set(name + ".outer", glm::cos(glm::radians(cutoff.outer)));
}

void Graphics::Shader::set(const std::string& name, Color color)
{
    set(name, color.vector());
}

void Graphics::Shader::set(const std::string& name, const Cubemap& cubemap, int id)
{
    glActiveTexture(GL_TEXTURE0 + id);
    cubemap.bind();
    set(name, id);
}

void Graphics::Shader::set(const std::string& name, const Texture& texture, int id)
{
    glActiveTexture(GL_TEXTURE0 + id);
    texture.bind();
    set(name, id);
}

} // namespace rf
