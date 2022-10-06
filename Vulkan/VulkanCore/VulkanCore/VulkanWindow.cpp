#include "pch.h"
#include "VulkanWindow.h"

VulkanWindow::VulkanWindow(const WindowProps& props)
{
    m_data.title = props.title;
    m_data.width = props.width;
    m_data.height = props.height;

    LOG_INFO("Initializing GLFW");
    if (glfwInit() == GLFW_FALSE)
    {
        LOG_ERROR("Failed to initialize GLFW");
        throw std::exception("Failed to initialize GLFW");
    }

    glfwSetErrorCallback([](int error, const char* description)
        {
            LOG_ERROR("GLFW error - {0}", description);
        }
    );

    LOG_INFO("Creating window {0} ({1}, {2})", m_data.title, m_data.width, m_data.height);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    // Create window
    if (props.debug)
    {
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        m_window = glfwCreateWindow((int)m_data.width, (int)m_data.height, m_data.title.c_str(), NULL, NULL);
    }
    else
    {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        m_data.width = mode->width;
        m_data.height = mode->height;
        m_window = glfwCreateWindow(mode->width, mode->height, m_data.title.c_str(), glfwGetPrimaryMonitor(), NULL);
    }

    if (!m_window)
    {
        LOG_ERROR("Failed to create window");
        throw std::exception("Failed to create window");
    }

    m_context = new VulkanContext(m_window);
    m_context->init();

    glfwSetWindowUserPointer(m_window, &m_data);

    glfwGetFramebufferSize(m_window, &m_data.framebufferWidth, &m_data.framebufferHeight);

    // Window events
    glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.eventCallback(event);
        }
    );

    glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
        {
            WindowData data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowResizeEvent event(width, height);
            data.eventCallback(event);
            data.resized = true;
        }
    );

    glfwSetWindowFocusCallback(m_window, [](GLFWwindow* window, int focused)
        {
            WindowData data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowFocusEvent event(focused != 0);
            data.eventCallback(event);
        }
    );

    // Key events
    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            if (action == GLFW_PRESS)
            {
                KeyDownEvent event(key);
                data.eventCallback(event);
            }
            else if (action == GLFW_RELEASE)
            {
                KeyUpEvent event(key);
                data.eventCallback(event);
            }
            else if (action == GLFW_REPEAT)
            {
                KeyDownEvent event(key, true);
                data.eventCallback(event);
            }
        }
    );

    // Mouse events
    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            MouseMoveEvent event(xPos, yPos);
            data.eventCallback(event);
        }
    );

    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            if (action == GLFW_PRESS)
            {
                MouseButtonDownEvent event(button);
                data.eventCallback(event);
            }
            else if (action == GLFW_RELEASE)
            {
                MouseButtonUpEvent event(button);
                data.eventCallback(event);
            }
        }
    );
}

VulkanWindow::~VulkanWindow()
{
    glfwSetWindowShouldClose(m_window, GLFW_TRUE);
    glfwDestroyWindow(m_window);
    m_context->terminate();
    delete m_context;
    LOG_INFO("Terminating GLFW");
    glfwTerminate();
}

void VulkanWindow::setEventCallback(const EventCallbackFn& callbackFn)
{
    m_data.eventCallback = callbackFn;
}

void VulkanWindow::update()
{
    glfwPollEvents();
    m_context->swapBuffers();
}

uint32_t VulkanWindow::getWidth() const
{
    return m_data.width;
}

uint32_t VulkanWindow::getHeight() const
{
    return m_data.height;
}
