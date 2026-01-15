#include "platform/window.hpp"
#include <stdexcept>

namespace niqqa
{
Window::Window(uint32_t w, uint32_t h, const char *name) : width(w), height(h), title(name)
{
    init_window();
}

Window::~Window()
{
    glfwDestroyWindow(window);
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

void Window::poll_events() const noexcept
{
    glfwPollEvents();
}

GLFWwindow *Window::get() const noexcept
{
    return window;
}
};