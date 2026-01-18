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
    Window(uint32_t width, uint32_t height, const char *title);
    ~Window();

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    VkSurfaceKHR create_surface(VkInstance instance) const;
    bool should_close() const noexcept;
    void poll_events() const noexcept;
    GLFWwindow *get_window_ptr() const noexcept;
    VkExtent2D get_extent() const noexcept; 

private:
    uint32_t m_width{0};
    uint32_t m_height{0};
    std::string m_title;

    GLFWwindow *m_window{nullptr}; 

    void init_window();
};
}