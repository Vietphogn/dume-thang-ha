#include "vulkan/sync_objects.hpp"

#include <stdexcept>

namespace niqqa
{
SyncObjects::SyncObjects(VkDevice device, uint32_t frames_in_flight, uint32_t swap_chain_image_count) : m_device(device)
{
    init_sync_objects(frames_in_flight, swap_chain_image_count);
}

SyncObjects::~SyncObjects()
{
    for (size_t i = 0; i < m_acquire_semaphores.size(); ++i)
    {
        vkDestroySemaphore(m_device, m_acquire_semaphores[i], nullptr);
        vkDestroyFence(m_device, m_frame_fences[i], nullptr);
    }

    for (const auto &semaphore : m_submit_semaphores)
    {
        vkDestroySemaphore(m_device, semaphore, nullptr);
    }
}

VkSemaphore SyncObjects::acquire_semaphore(uint32_t frame_index) const noexcept
{
    return m_acquire_semaphores[frame_index];
}

VkFence SyncObjects::frame_fence(uint32_t frame_index) const noexcept
{
    return m_frame_fences[frame_index];
}

VkSemaphore SyncObjects::submit_semaphore(uint32_t image_index) const noexcept
{
    return m_submit_semaphores[image_index];
}

void SyncObjects::init_sync_objects(uint32_t frames_in_flight, uint32_t swap_chain_image_count)
{
    m_acquire_semaphores.resize(frames_in_flight);
    m_frame_fences.resize(frames_in_flight);
    m_submit_semaphores.resize(swap_chain_image_count);

    VkSemaphoreCreateInfo semaphore_info{};
    semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fence_info{};
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < frames_in_flight; ++i)
    {
        if (vkCreateSemaphore(m_device, &semaphore_info, nullptr, &m_acquire_semaphores[i]) != VK_SUCCESS ||
            vkCreateFence(m_device, &fence_info, nullptr, &m_frame_fences[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create semaphore");
        }
    }

    for (size_t i = 0; i < swap_chain_image_count; ++i)
    {
        if (vkCreateSemaphore(m_device, &semaphore_info, nullptr, &m_submit_semaphores[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create semaphore");
        }
    }
}
}