#pragma once 

#include <vulkan/vulkan.h>
#include <vector>

namespace niqqa
{
class Instance 
{
public:
    Instance();
    ~Instance();

    VkInstance get() const noexcept;

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

    void init_instance();
    bool check_validation_support() noexcept;
};
}