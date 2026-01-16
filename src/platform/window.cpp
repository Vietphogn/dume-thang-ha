#include "platform/window.hpp"

#include <stdexcept>
#include <iostream>

namespace niqqa
{
Window::Window(uint32_t w, uint32_t h, const char *title) : width(w), height(h), title(title)
{
    init_window();
}

Window::~Window()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool Window::should_close() const noexcept
{
    return glfwWindowShouldClose(window);
}

void Window::init_window()
{
    if (!glfwInit())
    {
        throw std::runtime_error("failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (!window)
    {
        throw std::runtime_error("failed to create window");
    }
}

VkSurfaceKHR Window::create_surface(VkInstance instance) const
{
    VkSurfaceKHR surface;

    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create window surface");
    }

    return surface;
}

void Window::poll_events() const noexcept
{
    glfwPollEvents();
}

GLFWwindow *Window::get_window_ptr() const noexcept
{
    return window;
}

VkExtent2D Window::get_extent() const noexcept
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    VkExtent2D extent = {
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height)
    };

    return extent;
}
};