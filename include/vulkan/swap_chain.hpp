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
    SwapChain(VkDevice logical_device, VkPhysicalDevice physical_device, VkSurfaceKHR surface, VkExtent2D extent);
    ~SwapChain();

    void cleanup();
    void recreate();

    VkSwapchainKHR get_swap_chain() const noexcept;
    VkFormat get_image_format() const noexcept;
    VkExtent2D get_extent() const noexcept;
    uint32_t get_image_count() const noexcept;
    const std::vector<VkImageView> &get_image_views() const noexcept;

private:
    uint32_t swap_chain_image_count;

    VkSwapchainKHR swap_chain;
    std::vector<VkImage> swap_chain_images;
    VkFormat swap_chain_image_format;
    VkExtent2D swap_chain_extent;
    VkPhysicalDevice physical_device;
    VkDevice device;
    VkSurfaceKHR surface;

    std::vector<VkImageView> swap_chain_image_views;

    VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR> &available_formats) noexcept;
    VkPresentModeKHR choose_swap_present_mode(const std::vector<VkPresentModeKHR> &avaiable_present_modes) noexcept;
    VkExtent2D choose_swap_extent(const VkSurfaceCapabilitiesKHR &capabilities, VkExtent2D extent) noexcept;

    void init_swap_chain(VkExtent2D extent);
    void init_image_views();
};
}

