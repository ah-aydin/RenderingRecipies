#pragma once

#include "../Core.h"

class OpenGLShader
{
public:
    explicit OpenGLShader(const char* filename);
    OpenGLShader(GLenum type, const char* text, const char* debugFileName = "");
    ~OpenGLShader();

    OpenGLShader(const OpenGLShader&) = delete;
    OpenGLShader& operator=(const OpenGLShader&) = delete;

    GLenum getType() const { return m_type; }
    GLuint getHandle() const { return m_handle; }

private:
    GLenum m_type;
    GLuint m_handle;
};

