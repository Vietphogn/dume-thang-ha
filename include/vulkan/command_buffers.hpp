#pragma once

#include <vulkan/vulkan.h>
#include <cstdint>
#include <vector>

namespace niqqa
{
class CommandBuffers
{
public:
    CommandBuffers(VkDevice device, VkPhysicalDevice physical_device, VkSurfaceKHR surface, uint32_t max_frames_in_flight);
    ~CommandBuffers();

    void record(VkCommandBuffer command_buffer,
                VkFramebuffer framebuffer,
                VkRenderPass render_pass,
                VkExtent2D extent,
                VkPipeline graphics_pipeline);
    
    const std::vector<VkCommandBuffer> &get_command_buffer() const noexcept;

private:
    VkDevice m_device{VK_NULL_HANDLE};
    VkCommandPool m_command_pool{VK_NULL_HANDLE};
    std::vector<VkCommandBuffer> m_command_buffer;

    void init_command_pool(VkPhysicalDevice physical_device, VkSurfaceKHR surface);
    void init_command_buffer(uint32_t max_frames_in_flight);
};
}