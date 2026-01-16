#include "vulkan/device.hpp"
#include "vulkan/swap_chain.hpp"

#include <stdexcept>
#include <unordered_set>
#include <cstdint>
#include <string>
#include <iostream>

namespace niqqa
{
bool QueueFamilyIndices::is_complete()
{
    return graphics_family.has_value() && present_family.has_value();
}

QueueFamilyIndices find_queue_families(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    QueueFamilyIndices indices;

    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);

    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());
    
    int i = 0;
    for (const auto &queue_family : queue_families)
    {
        if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphics_family = i;
        }

        VkBool32 present_support = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &present_support);

        if (present_support)
        {
            indices.present_family = i;
        }

        if (indices.is_complete())
        {
            break;
        }

        ++i;
    }

    return indices;
}

Device::Device(VkInstance instance, VkSurfaceKHR surface)
{
    pick_physical_device(instance, surface);
    create_logical_device(surface);
}

Device::~Device()
{
    vkDeviceWaitIdle(logical_device);
    vkDestroyDevice(logical_device, nullptr);
}

VkPhysicalDevice Device::get_physical_device() const noexcept
{
    return physical_device;
}

VkDevice Device::get_logical_device() const noexcept
{
    return logical_device;
}

VkQueue Device::get_graphics_queue() const noexcept
{
    return graphics_queue;
}

VkQueue Device::get_present_queue() const noexcept
{
    return present_queue;
}

void Device::pick_physical_device(VkInstance instance, VkSurfaceKHR surface)
{
    uint32_t physical_device_count = 0;
    vkEnumeratePhysicalDevices(instance, &physical_device_count, nullptr);

    if (physical_device_count == 0)
    {
        throw std::runtime_error("failed to find GPUs with Vulkan support");
    }

    std::vector<VkPhysicalDevice> physical_devices(physical_device_count);
    vkEnumeratePhysicalDevices(instance, &physical_device_count, physical_devices.data());

    for (const auto &device : physical_devices)
    {
        if (is_device_suitable(device, surface))
        {
            physical_device = device;
            break;
        }
    }

    if (physical_device == VK_NULL_HANDLE)
    {
        throw std::runtime_error("failed to find a suitable GPU");
    }
}

void Device::create_logical_device(VkSurfaceKHR surface)
{
    QueueFamilyIndices indices = find_queue_families(physical_device, surface);

    std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
    std::unordered_set<uint32_t> unique_queue_families = {indices.graphics_family.value(), indices.present_family.value()};

    float queue_priority = 1.0f;
    for (uint32_t queue_family : unique_queue_families)
    {
        VkDeviceQueueCreateInfo queue_create_info{};
        queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.queueFamilyIndex = queue_family;
        queue_create_info.queueCount = 1;
        queue_create_info.pQueuePriorities = &queue_priority;
        queue_create_infos.push_back(queue_create_info);
    }

    VkPhysicalDeviceFeatures device_features{};

    VkDeviceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
    create_info.pQueueCreateInfos = queue_create_infos.data();
    create_info.pEnabledFeatures = &device_features;
    create_info.enabledExtensionCount = static_cast<uint32_t>(device_extensions.size());
    create_info.ppEnabledExtensionNames = device_extensions.data();

    if (vkCreateDevice(physical_device, &create_info, nullptr, &logical_device) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create logical device");
    }

    vkGetDeviceQueue(logical_device, indices.graphics_family.value(), 0, &graphics_queue);
    vkGetDeviceQueue(logical_device, indices.present_family.value(), 0, &present_queue);
}

bool Device::check_device_extension_support(VkPhysicalDevice device)
{
    uint32_t extension_count;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);
    
    std::vector<VkExtensionProperties> available_extensions(extension_count);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions.data());

    std::unordered_set<std::string> required_extensions(device_extensions.begin(), device_extensions.end());

    for (const auto &extension : available_extensions)
    {
        required_extensions.erase(extension.extensionName);
    }

    return required_extensions.empty();
}

bool Device::is_device_suitable(VkPhysicalDevice physical_device, VkSurfaceKHR surface)
{
    QueueFamilyIndices indices = find_queue_families(physical_device, surface);

    bool extensions_support = check_device_extension_support(physical_device);
    bool swap_chain_adequate = false;

    if (extensions_support)
    {
        SwapChainSupportDetails swap_chain_support = query_swap_chain_support(physical_device, surface);
        swap_chain_adequate = !swap_chain_support.formats.empty() && !swap_chain_support.present_modes.empty();        
    }

    return indices.is_complete() && extensions_support && swap_chain_adequate;
}
}