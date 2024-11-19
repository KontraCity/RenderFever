#include "rf/graphics/shader.hpp"

namespace rf {

std::string Graphics::Shader::ReadFile(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file)
        throw std::runtime_error(fmt::format("rf::Graphics::Shader::ReadFile(): Couldn't open file \"{}\"", filePath));

    std::stringstream stream;
    stream << file.rdbuf();
    return stream.str();
}

const char* Graphics::Shader::TypeToName(int type)
{
    switch (type)
    {
        case GL_VERTEX_SHADER:
            return "vertex";
        case GL_FRAGMENT_SHADER:
            return "fragment";
        default:
            return "unknown";
    }
}

unsigned int Graphics::Shader::CompileShader(const char* source, int type)
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

unsigned int Graphics::Shader::LinkShaderProgram(unsigned int vertexShader, unsigned int fragmentShader)
{
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
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

void Graphics::Shader::make(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
{
    free(); // avoid memory leaks if make() was called already
    m_vertexShader = CompileShader(ReadFile(vertexShaderFilePath).c_str(), GL_VERTEX_SHADER);
    m_fragmentShader = CompileShader(ReadFile(fragmentShaderFilePath).c_str(), GL_FRAGMENT_SHADER);
    m_shaderProgram = LinkShaderProgram(m_vertexShader, m_fragmentShader);
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

void Graphics::Shader::set(const std::string& name, const Texture& texture, int id)
{
    glActiveTexture(GL_TEXTURE0 + id);
    texture.bind();
    set(name, id);
}

} // namespace rf
