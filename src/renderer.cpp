#include "renderer.hpp"

#include <stdexcept>
#include <iostream>

namespace niqqa
{
Renderer::Renderer(VkExtent2D extent, VkSurfaceKHR surface, VkInstance instance) :
    surface(surface),
    device(instance, surface),
    swap_chain(device.get_logical_device(), device.get_physical_device(), surface, extent),
    sync_objects(device.get_logical_device(), MAX_FRAMES_IN_FLIGHT, swap_chain.get_image_count()),
    render_pass(device.get_logical_device(), swap_chain.get_image_format()),
    graphics_pipeline(device.get_logical_device(), swap_chain.get_extent(), render_pass.get()),
    framebuffers(device.get_logical_device(), render_pass.get(), swap_chain.get_extent(), swap_chain.get_image_views()),
    command_buffers(device.get_logical_device(), device.get_physical_device(), surface, MAX_FRAMES_IN_FLIGHT)
{
}

Renderer::~Renderer()
{
}

void Renderer::cleanup_swap_chain()
{
    framebuffers.cleanup();
    swap_chain.cleanup();
}

void Renderer::recreate_swap_chain()
{
    wait_idle();

    cleanup_swap_chain();

}

void Renderer::wait_idle() noexcept
{
    vkDeviceWaitIdle(device.get_logical_device());
}

void Renderer::draw_frame()
{
    VkFence frame_fence = sync_objects.frame_fence(current_frame);

    vkWaitForFences(device.get_logical_device(), 1, &frame_fence, VK_TRUE, UINT64_MAX);
    vkResetFences(device.get_logical_device(), 1, &frame_fence);

    VkSemaphore acquire_semaphore = sync_objects.acquire_semaphore(current_frame);

    uint32_t image_index;
    vkAcquireNextImageKHR(device.get_logical_device(), 
                          swap_chain.get_swap_chain(),
                          UINT64_MAX,
                          acquire_semaphore,
                          VK_NULL_HANDLE,
                          &image_index);

    vkResetCommandBuffer(command_buffers.get_command_buffer()[current_frame], 0);

    command_buffers.record(command_buffers.get_command_buffer()[current_frame],
                           framebuffers.get_framebuffers()[image_index],
                           render_pass.get(),
                           swap_chain.get_extent(), 
                           graphics_pipeline.get_graphics_pipeline());

    VkPipelineStageFlags wait_stages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkCommandBuffer command_buffer = command_buffers.get_command_buffer()[current_frame];
    VkSemaphore submit_semaphore = sync_objects.submit_semaphore(image_index);

    VkSubmitInfo submit_info{};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = &acquire_semaphore;
    submit_info.pWaitDstStageMask = &wait_stages;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffer;
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = &submit_semaphore;

    if (vkQueueSubmit(device.get_graphics_queue(), 1, &submit_info, frame_fence) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to submit draw command buffer");
    }

    VkPresentInfoKHR present_info{};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = &submit_semaphore;

    VkSwapchainKHR swap_chains = swap_chain.get_swap_chain();

    present_info.swapchainCount = 1;
    present_info.pSwapchains = &swap_chains;
    present_info.pImageIndices = &image_index;

    vkQueuePresentKHR(device.get_present_queue(), &present_info);

    current_frame = (current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
}
}