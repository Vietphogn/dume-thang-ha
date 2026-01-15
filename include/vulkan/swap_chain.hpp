#pragma once

#include "platform/window.hpp"

#include <vulkan/vulkan.h>
#include <vector>

namespace niqqa
{
struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;
};

SwapChainSupportDetails query_swap_chain_support(VkPhysicalDevice device, VkSurfaceKHR surface);

class SwapChain
{
public:
    SwapChain(VkDevice logical_device, VkPhysicalDevice physical_device, VkSurfaceKHR surface, const Window &window);
    ~SwapChain();

    VkFormat get_image_format() const noexcept;

private:
    VkSwapchainKHR swap_chain;
    std::vector<VkImage> swap_chain_images;
    VkFormat swap_chain_image_format;
    VkExtent2D swap_chain_extent;
    VkDevice device;

    std::vector<VkImageView> swap_chain_image_views;

    VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR> &available_formats) noexcept;
    VkPresentModeKHR choose_swap_present_mode(const std::vector<VkPresentModeKHR> &avaiable_present_modes) noexcept;
    VkExtent2D choose_swap_extent(const VkSurfaceCapabilitiesKHR &capabilities, const Window &window) noexcept;

    void init_swap_chain(VkDevice logical_device, VkPhysicalDevice physical_device, VkSurfaceKHR surface, const Window &window);
    void init_image_views();
};
}

