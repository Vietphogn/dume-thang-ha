#include "vulkan/command_buffers.hpp"

#include "vulkan/device.hpp"
#include <stdexcept>
#include <iostream>

namespace niqqa
{
CommandBuffers::CommandBuffers(VkDevice _device, VkPhysicalDevice physical_device, VkSurfaceKHR surface, uint32_t max_frames_in_flight)
    : device(_device)
{
    init_command_pool(physical_device, surface);
    init_command_buffer(max_frames_in_flight);
}

CommandBuffers::~CommandBuffers()
{
    vkDestroyCommandPool(device, command_pool, nullptr);

    //std::cout << "~CommandBuffers()\n";
}

void CommandBuffers::record(VkCommandBuffer command_buffer,
                            VkFramebuffer framebuffer,
                            VkRenderPass render_pass,
                            VkExtent2D extent,
                            VkPipeline graphics_pipeline)
{
    VkCommandBufferBeginInfo begin_info{};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(command_buffer, &begin_info) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to begin command buffer");
    }

    VkRenderPassBeginInfo render_pass_info{};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_info.renderPass = render_pass;
    render_pass_info.framebuffer = framebuffer;
    render_pass_info.renderArea.offset = {0, 0};
    render_pass_info.renderArea.extent = extent;

    VkClearValue clear_color = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    render_pass_info.clearValueCount = 1;
    render_pass_info.pClearValues = &clear_color;

    vkCmdBeginRenderPass(command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(extent.width);
    viewport.height = static_cast<float>(extent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    vkCmdSetViewport(command_buffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = extent;
    
    vkCmdSetScissor(command_buffer, 0, 1, &scissor);

    vkCmdDraw(command_buffer, 3, 1, 0, 0);

    vkCmdEndRenderPass(command_buffer);

    if (vkEndCommandBuffer(command_buffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to record command buffer");
    }
}

const std::vector<VkCommandBuffer> &CommandBuffers::get_command_buffer() const noexcept
{
    return command_buffer;
}

void CommandBuffers::init_command_pool(VkPhysicalDevice physical_device, VkSurfaceKHR surface)
{
    QueueFamilyIndices queue_family_indices = find_queue_families(physical_device, surface);

    VkCommandPoolCreateInfo pool_info{};
    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    pool_info.queueFamilyIndex = queue_family_indices.graphics_family.value();

    if (vkCreateCommandPool(device, &pool_info, nullptr, &command_pool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create command pool");
    }
}

void CommandBuffers::init_command_buffer(uint32_t max_frames_in_flight)
{
    command_buffer.resize(max_frames_in_flight);

    VkCommandBufferAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.commandPool = command_pool;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandBufferCount = static_cast<uint32_t>(command_buffer.size());

    if (vkAllocateCommandBuffers(device, &alloc_info, command_buffer.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate command buffers");
    }
}
}