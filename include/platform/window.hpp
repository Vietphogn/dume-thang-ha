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
    Window(int width, int height, const char *title);
    ~Window();

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    VkSurfaceKHR create_surface(VkInstance instance) const;
    bool should_close() const noexcept;
    bool get_framebuffer_resized() const noexcept;
    bool is_minimized() const noexcept;
    void poll_events() const noexcept;
    void reset_framebuffer_resized() noexcept;
    void wait_events() const noexcept;
    GLFWwindow *get_window_ptr() const noexcept;
    VkExtent2D get_extent() const noexcept; 

private:
    int m_width{0};
    int m_height{0};
    std::string m_title;
    bool m_framebuffer_resized{false};

    GLFWwindow *m_window{nullptr}; 

    void init_window();
    static void framebuffer_resize_callback(GLFWwindow *window, int width, int height);
};
}