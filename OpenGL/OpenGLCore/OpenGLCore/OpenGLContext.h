#pragma once

#include <Core/Core.h>
#include "Core.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

class OpenGLContext
{
public:
    OpenGLContext(const OpenGLContext&) = delete;
    OpenGLContext& operator=(const OpenGLContext&) = delete;

    OpenGLContext(GLFWwindow* windowHandle);

    void init();
    void swapBuffers() const;
    void terminate();
private:
    GLFWwindow* m_windowHandle;
};
