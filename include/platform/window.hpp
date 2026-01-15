#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <cstdint>

namespace niqqa
{
class Window
{
public:
    Window(uint32_t w, uint32_t h, const char *name);
    ~Window();

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    bool should_close() const noexcept;
    void poll_events() const noexcept;
    GLFWwindow *get() const noexcept;

private:
    uint32_t width, height;
    std::string title;

    GLFWwindow *window = nullptr; 

    void init_window();
};
}