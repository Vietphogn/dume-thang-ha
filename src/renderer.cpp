#include "renderer.hpp"

#include <stdexcept>
#include <iostream>

namespace niqqa
{
Renderer::Renderer(VkExtent2D extent, VkSurfaceKHR surface, VkInstance instance) :
    m_surface(surface),
    m_device(instance, surface),
    m_swap_chain(m_device.get_logical_device(), m_device.get_physical_device(), surface, extent),
    m_sync_objects(m_device.get_logical_device(), s_max_frames_in_flight, m_swap_chain.get_image_count()),
    m_render_pass(m_device.get_logical_device(), m_swap_chain.get_image_format()),
    m_graphics_pipeline(m_device.get_logical_device(), m_swap_chain.get_extent(), m_render_pass.get_render_pass()),
    m_framebuffers(m_device.get_logical_device(), m_render_pass.get_render_pass(), m_swap_chain.get_extent(), m_swap_chain.get_image_views()),
    m_command_buffers(m_device.get_logical_device(), m_device.get_physical_device(), surface, s_max_frames_in_flight)
{
}

Renderer::~Renderer()
{
}

void Renderer::cleanup_swap_chain() noexcept
{
    m_framebuffers.cleanup();
    m_swap_chain.cleanup();
}

void Renderer::recreate_swap_chain()
{
    wait_idle();

    cleanup_swap_chain();

}

void Renderer::wait_idle() noexcept
{
    vkDeviceWaitIdle(m_device.get_logical_device());
}

void Renderer::draw_frame()
{
    VkFence frame_fence = m_sync_objects.frame_fence(m_current_frame);

    vkWaitForFences(m_device.get_logical_device(), 1, &frame_fence, VK_TRUE, UINT64_MAX);
    vkResetFences(m_device.get_logical_device(), 1, &frame_fence);

    VkSemaphore acquire_semaphore = m_sync_objects.acquire_semaphore(m_current_frame);

    uint32_t image_index;
    vkAcquireNextImageKHR(m_device.get_logical_device(), 
                          m_swap_chain.get_swap_chain(),
                          UINT64_MAX,
                          acquire_semaphore,
                          VK_NULL_HANDLE,
                          &image_index);

    vkResetCommandBuffer(m_command_buffers.get_command_buffer()[m_current_frame], 0);

    m_command_buffers.record(m_command_buffers.get_command_buffer()[m_current_frame],
                           m_framebuffers.get_framebuffers()[image_index],
                           m_render_pass.get_render_pass(),
                           m_swap_chain.get_extent(), 
                           m_graphics_pipeline.get_graphics_pipeline());

    VkPipelineStageFlags wait_stages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkCommandBuffer command_buffer = m_command_buffers.get_command_buffer()[m_current_frame];
    VkSemaphore submit_semaphore = m_sync_objects.submit_semaphore(image_index);

    VkSubmitInfo submit_info{};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = &acquire_semaphore;
    submit_info.pWaitDstStageMask = &wait_stages;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffer;
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = &submit_semaphore;

    if (vkQueueSubmit(m_device.get_graphics_queue(), 1, &submit_info, frame_fence) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to submit draw command buffer");
    }

    VkPresentInfoKHR present_info{};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = &submit_semaphore;

    VkSwapchainKHR swap_chains = m_swap_chain.get_swap_chain();

    present_info.swapchainCount = 1;
    present_info.pSwapchains = &swap_chains;
    present_info.pImageIndices = &image_index;

    vkQueuePresentKHR(m_device.get_present_queue(), &present_info);

    m_current_frame = (m_current_frame + 1) % s_max_frames_in_flight;
}
}