#include "platform/window.hpp"

#include <stdexcept>
#include <iostream>

namespace niqqa
{
Window::Window(int w, int h, const char *title) : m_width(w), m_height(h), m_title(title)
{
    init_window();
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

bool Window::should_close() const noexcept
{
    return glfwWindowShouldClose(m_window);
}

bool Window::get_framebuffer_resized() const noexcept
{
    return m_framebuffer_resized;
}

bool Window::is_minimized() const noexcept
{
    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);

    return (width == 0 || height == 0);
}

void Window::init_window()
{
    if (!glfwInit())
    {
        throw std::runtime_error("failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    m_window = glfwCreateWindow(static_cast<int>(m_width), static_cast<int>(m_height), m_title.c_str(), nullptr, nullptr);
    if (!m_window)
    {
        throw std::runtime_error("failed to create window");
    }

    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_resize_callback);
}

VkSurfaceKHR Window::create_surface(VkInstance instance) const
{
    VkSurfaceKHR surface;

    if (glfwCreateWindowSurface(instance, m_window, nullptr, &surface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create window surface");
    }

    return surface;
}

void Window::poll_events() const noexcept
{
    glfwPollEvents();
}

void Window::reset_framebuffer_resized() noexcept
{
    m_framebuffer_resized = false;    
}

void Window::wait_events() const noexcept
{
    glfwWaitEvents();
}

GLFWwindow *Window::get_window_ptr() const noexcept
{
    return m_window;
}

VkExtent2D Window::get_extent() const noexcept
{
    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);

    VkExtent2D extent = {
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height)
    };

    return extent;
}

void Window::framebuffer_resize_callback(GLFWwindow *window, int width, int height)
{
    Window *self = static_cast<Window *>(glfwGetWindowUserPointer(window));
    self->m_framebuffer_resized = true;
}
};