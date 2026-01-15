#include "vulkan/instance.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstdint>
#include <stdexcept>
#include <cstring>
#include "vulkan/instance.hpp"

namespace niqqa
{
Instance::Instance()
{
    init_instance();
}

Instance::~Instance()
{
    vkDestroyInstance(instance, nullptr);
}

VkInstance Instance::get() const noexcept
{
    return instance;
}

void Instance::init_instance()
{
    if (enable_validation_layers && !check_validation_support())
    {
        throw std::runtime_error("validation layers requested, but not available");
    }

    VkApplicationInfo app_info{};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "Engine";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "No Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_0;

    uint32_t glfw_extension_count = 0;
    const char **glfw_extensions;

    glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

    VkInstanceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;
    create_info.enabledExtensionCount = glfw_extension_count;
    create_info.ppEnabledExtensionNames = glfw_extensions;
    create_info.enabledLayerCount = 0;

    if (enable_validation_layers)
    {
        create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
        create_info.ppEnabledLayerNames = validation_layers.data();
    }

    if (vkCreateInstance(&create_info, nullptr, &instance) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create vulkan instance");
    }
}

bool Instance::check_validation_support() noexcept
{
    uint32_t layer_count;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

    std::vector<VkLayerProperties> available_layers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

    for (const char *layer_name : validation_layers)
    {
        bool layer_found = false;

        for (const auto &layer_properties : available_layers)
        {
            if (strcmp(layer_name, layer_properties.layerName) == 0)
            {
                layer_found = true;
                break;
            }
        }

        if (!layer_found)
        {
            return false;
        }
    }

    return true;
}
}