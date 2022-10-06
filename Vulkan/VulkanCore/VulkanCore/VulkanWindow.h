#pragma once

#include <Core/Core.h>
#include <Core/Event/Event.h>
#include "VulkanContext.h"

#include <GLFW/glfw3.h>

struct WindowProps
{
    std::string title;
    uint32_t width;
    uint32_t height;
    bool debug;

    explicit WindowProps(const std::string title = "Vulkan Window", uint32_t width = 1280, uint32_t height = 800, bool debug = false)
        : title(title), width(width), height(height), debug(debug)
    {
    }
};

class VulkanWindow
{
public:
    using EventCallbackFn = std::function<void(Event&)>;

    VulkanWindow(const WindowProps& props);
    ~VulkanWindow();

    void update();

    uint32_t getWidth() const;
    uint32_t getHeight() const;
    VkExtent2D getExtent() { return { getWidth(), getHeight() }; }
    bool wasResized() { return m_data.resized; }
    void resetResizedFlag() { m_data.resized = false; }

    void setEventCallback(const EventCallbackFn& callbackFn);

    VulkanContext* getContext() { return m_context; }

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
        bool resized;
    };

    WindowData m_data;

    GLFWwindow* m_window;

    VulkanContext* m_context;
};
