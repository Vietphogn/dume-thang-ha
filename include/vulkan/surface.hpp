#pragma once

#include "platform/window.hpp"
#include <vulkan/vulkan.h>

namespace niqqa
{
class Surface
{
public:
    Surface(VkInstance _instance, const Window &window);
    ~Surface();

    VkSurfaceKHR get() const noexcept;

private:
    VkInstance instance;
    VkSurfaceKHR surface;    

    void init_instance(const Window &window);
};
}