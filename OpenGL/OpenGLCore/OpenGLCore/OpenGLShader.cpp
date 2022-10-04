#include "pch.h"
#include "OpenGLShader.h"

int endsWith(const char* s, const char* part)
{
    return strstr(s, part) - s == strlen(s) - strlen(part);
}

GLenum shaderTypeFromFileName(const char* filename)
{
    if (endsWith(filename, ".vert")) return GL_VERTEX_SHADER;
    if (endsWith(filename, ".frag")) return GL_FRAGMENT_SHADER;
    if (endsWith(filename, ".geom")) return GL_GEOMETRY_SHADER;
    if (endsWith(filename, ".tesc")) return GL_TESS_CONTROL_SHADER;
    if (endsWith(filename, ".tese")) return GL_TESS_EVALUATION_SHADER;
    if (endsWith(filename, ".comp")) return GL_COMPUTE_SHADER;

    assert(false);

    return 0;
}

std::string readShaderFile(const char* filename)
{
    FILE* file;
    fopen_s(&file, filename, "r");

    if (!file)
    {
        LOG_ERROR("Failed to open shader file {0}", filename);
        return std::string();
    }

    fseek(file, 0L, SEEK_END);
    const long bytesinfile = ftell(file);
    fseek(file, 0L, SEEK_SET);

    char* buffer = (char*)_malloca(static_cast<size_t>(bytesinfile) + 1);
    if (!buffer)
    {
        return std::string();
    }

    const size_t bytesread = fread(buffer, 1, bytesinfile, file);
    fclose(file);

    buffer[bytesread] = 0;

    static constexpr unsigned char BOM[] = { 0xEF, 0xBB, 0xBF };

    std::string code(buffer);

    while (code.find("#include ") != code.npos)
    {
        const auto pos = code.find("#include ");
        const auto p1 = code.find('<', pos);
        const auto p2 = code.find('>', pos);
        if (p1 == code.npos || p2 == code.npos || p1 >= p2)
        {
            LOG_ERROR("Failed to inclue statemtn in shader file {0}", filename);
            return std::string();
        }
        const std::string name = code.substr(p1 + 1, p2 - p1 - 1);
        const std::string include = readShaderFile(name.c_str());
        code.replace(pos, p2 - pos + 1, include.c_str());
    }

    return code;
}

OpenGLShader::OpenGLShader(const char* filename)
    : OpenGLShader(shaderTypeFromFileName(filename), readShaderFile(filename).c_str(), filename)
{}

OpenGLShader::OpenGLShader(GLenum type, const char* text, const char* debugFileName)
    : m_type(type), m_handle(glCreateShader(type))
{
    GL_CALL(glShaderSource(m_handle, 1, &text, nullptr));
    GL_CALL(glCompileShader(m_handle));

    char buffer[512];
    GLsizei length = 0;
    GL_CALL(glGetShaderInfoLog(m_handle, sizeof(buffer), &length, buffer));

    if (length)
    {
        LOG_ASSERTION(false, "Shader compilation error {0} (File: {1})", buffer, debugFileName);
    }
}

OpenGLShader::~OpenGLShader()
{
    GL_CALL(glDeleteShader(m_handle));
}
