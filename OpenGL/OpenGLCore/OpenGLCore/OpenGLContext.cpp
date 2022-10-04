#include "pch.h"
#include "OpenGLContext.h"

OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
    : m_windowHandle(windowHandle)
{}

void OpenGLContext::init()
{
    LOG_INFO("Initializing OpenGL context");
    LOG_INFO("\tInitializing GLAD");
    glfwMakeContextCurrent(m_windowHandle);
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (!status)
    {
        LOG_ERROR("GLAD initialization failed");
    }
    LOG_INFO("\tOpenGL vendor: {0}", (const char*)glGetString(GL_VENDOR));
    LOG_INFO("\tOpenGL renderer: {0}", (const char*)glGetString(GL_RENDERER));
    LOG_INFO("\tOpenGL version: {0}", (const char*)glGetString(GL_VERSION));

    GL_CALL(glEnable(GL_DEPTH_TEST));
    GL_CALL(glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS));
}

void OpenGLContext::swapBuffers() const
{
    glfwSwapBuffers(m_windowHandle);
}

void OpenGLContext::terminate()
{

}
