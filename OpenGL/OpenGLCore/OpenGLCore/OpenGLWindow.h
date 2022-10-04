#pragma once

#include <Core/Core.h>
#include <Core/Event/Event.h>
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>

struct WindowProps
{
    std::string title;
    uint32_t width;
    uint32_t height;
    bool debug;

    explicit WindowProps(const std::string title = "OpenGL Window", uint32_t width = 1280, uint32_t height = 800, bool debug = false)
        : title(title), width(width), height(height), debug(debug)
    {
    }
};

class Window
{
public:
    using EventCallbackFn = std::function<void(Event&)>;

    Window(const WindowProps& props);
    ~Window();

    void update();

    uint32_t getWidth() const;
    uint32_t getHeight() const;

    void setEventCallback(const EventCallbackFn& callbackFn);

    GLFWwindow* getHandle() { return m_window; }
private:
    struct WindowData
    {
        std::string title;
        uint32_t width;
        uint32_t height;
        int32_t framebufferWidth;
        int32_t framebufferHeight;
        EventCallbackFn eventCallback;
    };

    WindowData m_data;

    GLFWwindow* m_window;

    OpenGLContext* m_context;
};
