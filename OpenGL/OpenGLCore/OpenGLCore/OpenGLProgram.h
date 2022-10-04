#pragma once

#include "OpenGLShader.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>

class OpenGLProgram
{
public:
    OpenGLProgram(const char *filename1, const char *filename2);
    OpenGLProgram(const char *filename1, const char *filename2, const char *filename3);
    OpenGLProgram(const char *filename1, const char *filename2, const char *filename3, const char *filename4);
    OpenGLProgram(const char *filename1, const char *filename2, const char *filename3, const char *filename4, const char *filename5);

    OpenGLProgram(const OpenGLShader &s1, const OpenGLShader &s2);
    OpenGLProgram(const OpenGLShader &s1, const OpenGLShader &s2, const OpenGLShader &s3);
    OpenGLProgram(const OpenGLShader &s1, const OpenGLShader &s2, const OpenGLShader &s3, const OpenGLShader &s4);
    OpenGLProgram(const OpenGLShader &s1, const OpenGLShader &s2, const OpenGLShader &s3, const OpenGLShader &s4, const OpenGLShader &s5);

    void use() const;

    GLuint getHandle() const { return m_handle; }

    // Uniform setters
    void setInt(const char* uniformName, GLint v0);
    void setFloat(const char* uniformName, GLfloat v0);
    void setVector2f(const char* uniformName, GLfloat v0, GLfloat v1);
    void setVector2f(const char* uniformName, glm::vec2 vec);
    void setVector3f(const char* uniformName, GLfloat v0, GLfloat v1, GLfloat v2);
    void setVector3f(const char* uniformName, glm::vec3 vec);
    void setVector4f(const char* uniformName, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
    void setVector4f(const char* uniformName, glm::vec4 vec);
    void setMatrix2f(
        const char* uniformName,
        GLfloat x0, GLfloat x1,
        GLfloat y0, GLfloat y1
    );
    void setMatrix2f(const char* uniformName, glm::mat2 mat);
    void setMatrix3f(
        const char* uniformName,
        GLfloat x0, GLfloat x1, GLfloat x2,
        GLfloat y0, GLfloat y1, GLfloat y2,
        GLfloat z0, GLfloat z1, GLfloat z2
    );
    void setMatrix3f(const char* uniformName, glm::mat3 mat);
    void setMatrix4f(
        const char* uniformName,
        GLfloat x0, GLfloat x1, GLfloat x2, GLfloat x3,
        GLfloat y0, GLfloat y1, GLfloat y2, GLfloat y3,
        GLfloat z0, GLfloat z1, GLfloat z2, GLfloat z3,
        GLfloat w0, GLfloat w1, GLfloat w2, GLfloat w3
    );
    void setMatrix4f(const char* uniformName, glm::mat4 mat);

private:
    GLuint m_handle;

    void attachShader(const OpenGLShader &shader);
    void detachShader(const OpenGLShader &shader);

    void printProgramInfoLog();

    std::map<const char*, int> m_uniforms;
    GLint getUniformLocation(const char* uniformName);
};

