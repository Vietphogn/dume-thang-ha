#pragma once 

#include "platform/window.hpp"

#include <vulkan/vulkan.h>
#include <vector>

namespace niqqa
{
class Instance 
{
public:
    Instance(const Window &window);
    ~Instance();

    VkInstance get_instance() const noexcept;
    VkSurfaceKHR get_surface() const noexcept;

private:
    const std::vector<const char *> validation_layers = {
        "VK_LAYER_KHRONOS_validation"
    };

#ifdef NDEBUG
    static constexpr bool enable_validation_layers = false;
#else
    static constexpr bool enable_validation_layers = true;
#endif

    VkInstance instance;
    VkSurfaceKHR surface;

    void init_instance();
    bool check_validation_support() noexcept;
};
}