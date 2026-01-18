#pragma once

#include <vulkan/vulkan.h>
#include <cstdint>
#include <optional>
#include <vector>

namespace niqqa
{
struct QueueFamilyIndices
{
    std::optional<uint32_t> graphics_family;
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
    VkQueue get_graphics_queue() const noexcept;
    VkQueue get_present_queue() const noexcept;

private:
    const std::vector<const char *> device_extensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    VkPhysicalDevice m_physical_device{VK_NULL_HANDLE};
    VkDevice m_logical_device{VK_NULL_HANDLE};
    VkQueue m_graphics_queue{VK_NULL_HANDLE};
    VkQueue m_present_queue{VK_NULL_HANDLE};

    void pick_physical_device(VkInstance instance, VkSurfaceKHR surface);
    void create_logical_device(VkSurfaceKHR surface);
    bool check_device_extension_support(VkPhysicalDevice device);
    bool is_device_suitable(VkPhysicalDevice device, VkSurfaceKHR surface);
};
}