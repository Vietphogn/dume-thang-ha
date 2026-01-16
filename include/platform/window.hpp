#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>
#include <string>
#include <cstdint>

namespace niqqa
{
class Window
{
public:
    Window(uint32_t w, uint32_t h, const char *title);
    ~Window();

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    VkSurfaceKHR create_surface(VkInstance instance) const;
    bool should_close() const noexcept;
    void poll_events() const noexcept;
    GLFWwindow *get_window_ptr() const noexcept;
    VkExtent2D get_extent() const noexcept; 

private:
    uint32_t width, height;
    std::string title;

    GLFWwindow *window = nullptr; 

    void init_window();
};
}