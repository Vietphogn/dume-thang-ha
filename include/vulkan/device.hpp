#pragma once

#include <vulkan/vulkan.h>
#include <cstdint>
#include <optional>
#include <vector>

namespace niqqa
{
struct QueueFamilyIndices
{
    std::optional<uint32_t> graphics_famlily;
    std::optional<uint32_t> present_family;

    bool is_complete();
};

QueueFamilyIndices find_queue_families(VkPhysicalDevice device, VkSurfaceKHR surface);

class Device
{
public:
    Device(VkInstance instance, VkSurfaceKHR surface);
    ~Device();

    VkPhysicalDevice get_physical_device() const noexcept;
    VkDevice get_logical_device() const noexcept;

private:
    const std::vector<const char *> device_extensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    VkPhysicalDevice physical_device = VK_NULL_HANDLE;
    VkDevice logical_device = VK_NULL_HANDLE;
    VkQueue graphics_queue;
    VkQueue present_queue;

    void pick_physical_device(VkInstance instance, VkSurfaceKHR surface);
    void create_logical_device(VkSurfaceKHR surface);
    bool check_device_extension_support(VkPhysicalDevice device);
    bool is_device_suitable(VkPhysicalDevice physical_device, VkSurfaceKHR surface);
};
}