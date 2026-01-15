#include "vulkan/surface.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>

namespace niqqa
{
Surface::Surface(VkInstance _instance, const Window &window) : instance(_instance)
{
    init_instance(window);
}

Surface::~Surface()
{
    vkDestroySurfaceKHR(instance, surface, nullptr);
}

VkSurfaceKHR Surface::get() const noexcept
{
    return surface;
}

void Surface::init_instance(const Window &window)
{
    if (glfwCreateWindowSurface(instance, window.get(), nullptr, &surface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create window surface");
    }
}
}