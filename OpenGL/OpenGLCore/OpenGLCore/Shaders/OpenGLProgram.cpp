#include "pch.h"
#include "OpenGLProgram.h"

OpenGLProgram::OpenGLProgram(const char* filename1, const char* filename2)
    : OpenGLProgram(OpenGLShader(filename1), OpenGLShader(filename2))
{
}

OpenGLProgram::OpenGLProgram(const char* filename1, const char* filename2, const char* filename3)
    : OpenGLProgram(OpenGLShader(filename1), OpenGLShader(filename2), OpenGLShader(filename3))
{
}

OpenGLProgram::OpenGLProgram(const char* filename1, const char* filename2, const char* filename3, const char* filename4)
    : OpenGLProgram(OpenGLShader(filename1), OpenGLShader(filename2), OpenGLShader(filename3), OpenGLShader(filename4))
{
}

OpenGLProgram::OpenGLProgram(const char* filename1, const char* filename2, const char* filename3, const char* filename4, const char* filename5)
    : OpenGLProgram(OpenGLShader(filename1), OpenGLShader(filename2), OpenGLShader(filename3), OpenGLShader(filename4), OpenGLShader(filename5))
{
}

OpenGLProgram::OpenGLProgram(const OpenGLShader& s1, const OpenGLShader& s2)
    : m_handle(glCreateProgram())
{
    attachShader(s1);
    attachShader(s2);
    GL_CALL(glLinkProgram(m_handle));
    printProgramInfoLog();
    detachShader(s1);
    detachShader(s2);
}

OpenGLProgram::OpenGLProgram(const OpenGLShader& s1, const OpenGLShader& s2, const OpenGLShader& s3)
    : m_handle(glCreateProgram())
{
    attachShader(s1);
    attachShader(s2);
    attachShader(s3);
    GL_CALL(glLinkProgram(m_handle));
    printProgramInfoLog();
    detachShader(s1);
    detachShader(s2);
    detachShader(s3);
}

OpenGLProgram::OpenGLProgram(const OpenGLShader& s1, const OpenGLShader& s2, const OpenGLShader& s3, const OpenGLShader& s4)
    : m_handle(glCreateProgram())
{
    attachShader(s1);
    attachShader(s2);
    attachShader(s3);
    attachShader(s4);
    GL_CALL(glLinkProgram(m_handle));
    printProgramInfoLog();
    detachShader(s1);
    detachShader(s2);
    detachShader(s3);
    detachShader(s4);
}

OpenGLProgram::OpenGLProgram(const OpenGLShader& s1, const OpenGLShader& s2, const OpenGLShader& s3, const OpenGLShader& s4, const OpenGLShader& s5)
    : m_handle(glCreateProgram())
{
    attachShader(s1);
    attachShader(s2);
    attachShader(s3);
    attachShader(s4);
    attachShader(s5);
    GL_CALL(glLinkProgram(m_handle));
    printProgramInfoLog();
    detachShader(s1);
    detachShader(s2);
    detachShader(s3);
    detachShader(s4);
    detachShader(s5);
}

void OpenGLProgram::use() const
{
    GL_CALL(glUseProgram(m_handle));
}

void OpenGLProgram::attachShader(const OpenGLShader& shader)
{
    GL_CALL(glAttachShader(m_handle, shader.getHandle()));
}

void OpenGLProgram::detachShader(const OpenGLShader& shader)
{
    GL_CALL(glDetachShader(m_handle, shader.getHandle()));
}

void OpenGLProgram::printProgramInfoLog()
{
    char buffer[8192];
    GLsizei length = 0;
    GL_CALL(glGetProgramInfoLog(m_handle, sizeof(buffer), &length, buffer));
    if (length)
    {
        LOG_ASSERTION(false, "Shader program linking: {0}", buffer);
    }
}

void OpenGLProgram::setInt(const char* uniformName, GLint v0)
{
    use();
    GL_CALL(glUniform1i(getUniformLocation(uniformName), v0));
}

void OpenGLProgram::setFloat(const char* uniformName, GLfloat v0)
{
    use();
    GL_CALL(glUniform1f(getUniformLocation(uniformName), v0));
}

void OpenGLProgram::setVector2f(const char* uniformName, GLfloat v0, GLfloat v1)
{
    use();
    GL_CALL(glUniform2f(getUniformLocation(uniformName), v0, v1));
}
void OpenGLProgram::setVector2f(const char* uniformName, glm::vec2 vec)
{
    setVector2f(uniformName, vec.x, vec.y);
}

void OpenGLProgram::setVector3f(const char* uniformName, GLfloat v0, GLfloat v1, GLfloat v2)
{
    use();
    GL_CALL(glUniform3f(getUniformLocation(uniformName), v0, v1, v2));
}
void OpenGLProgram::setVector3f(const char* uniformName, glm::vec3 vec)
{
    setVector3f(uniformName, vec.x, vec.y, vec.z);
}

void OpenGLProgram::setVector4f(const char* uniformName, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
    use();
    GL_CALL(glUniform4f(getUniformLocation(uniformName), v0, v1, v2, v3));
}
void OpenGLProgram::setVector4f(const char* uniformName, glm::vec4 vec)
{
    setVector4f(uniformName, vec.x, vec.y, vec.z, vec.w);
}

void OpenGLProgram::setMatrix2f(
    const char* uniformName,
    GLfloat x0, GLfloat x1,
    GLfloat y0, GLfloat y1
)
{
    setMatrix2f(uniformName, glm::mat2(x0, y0, x1, y1));
}
void OpenGLProgram::setMatrix2f(const char* uniformName, glm::mat2 mat)
{
    use();
    GL_CALL(glUniformMatrix2fv(getUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(mat)));
}

void OpenGLProgram::setMatrix3f(
    const char* uniformName,
    GLfloat x0, GLfloat x1, GLfloat x2,
    GLfloat y0, GLfloat y1, GLfloat y2,
    GLfloat z0, GLfloat z1, GLfloat z2
)
{
    setMatrix3f(uniformName, glm::mat3(x0, y0, z0, x1, y1, z1, x2, y2, z2));
}
void OpenGLProgram::setMatrix3f(const char* uniformName, glm::mat3 mat)
{
    use();
    GL_CALL(glUniformMatrix3fv(getUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(mat)));
}

void OpenGLProgram::setMatrix4f(
    const char* uniformName,
    GLfloat x0, GLfloat x1, GLfloat x2, GLfloat x3,
    GLfloat y0, GLfloat y1, GLfloat y2, GLfloat y3,
    GLfloat z0, GLfloat z1, GLfloat z2, GLfloat z3,
    GLfloat w0, GLfloat w1, GLfloat w2, GLfloat w3
)
{
    setMatrix4f(uniformName, glm::mat4(x0, y0, z0, w0, x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3));
}
void OpenGLProgram::setMatrix4f(const char* uniformName, glm::mat4 mat)
{
    use();
    GL_CALL(glUniformMatrix4fv(getUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(mat)));
}

GLint OpenGLProgram::getUniformLocation(const char* uniformName)
{
    std::map<const char*, int>::iterator it = m_uniforms.find(uniformName);
    if (it != m_uniforms.end())
    {
        return it->second;
    }

    GLint location = glGetUniformLocation(m_handle, uniformName);
    if (location == -1)
    {
        LOG_ASSERTION(false, "Invalid uniform name {0}", uniformName);
    }
    m_uniforms.insert(std::pair<const char*, int>(uniformName, location));
    return location;
}

