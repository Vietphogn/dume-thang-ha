#include "vulkan/sync_objects.hpp"

#include <stdexcept>

namespace niqqa
{
SyncObjects::SyncObjects(VkDevice device, uint32_t frames_in_flight, uint32_t swap_chain_image_count) : device(device)
{
    init_sync_objects(frames_in_flight, swap_chain_image_count);
}

SyncObjects::~SyncObjects()
{
    for (size_t i = 0; i < acquire_semaphores.size(); ++i)
    {
        vkDestroySemaphore(device, acquire_semaphores[i], nullptr);
        vkDestroyFence(device, frame_fences[i], nullptr);
    }

    for (const auto &semaphore : submit_semaphores)
    {
        vkDestroySemaphore(device, semaphore, nullptr);
    }
}

VkSemaphore SyncObjects::acquire_semaphore(uint32_t frame_index) const noexcept
{
    return acquire_semaphores[frame_index];
}

VkFence SyncObjects::frame_fence(uint32_t frame_index) const noexcept
{
    return frame_fences[frame_index];
}

VkSemaphore SyncObjects::submit_semaphore(uint32_t image_index) const noexcept
{
    return submit_semaphores[image_index];
}

void SyncObjects::init_sync_objects(uint32_t frames_in_flight, uint32_t swap_chain_image_count)
{
    acquire_semaphores.resize(frames_in_flight);
    frame_fences.resize(frames_in_flight);
    submit_semaphores.resize(swap_chain_image_count);

    VkSemaphoreCreateInfo semaphore_info{};
    semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fence_info{};
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < frames_in_flight; ++i)
    {
        if (vkCreateSemaphore(device, &semaphore_info, nullptr, &acquire_semaphores[i]) != VK_SUCCESS ||
            vkCreateFence(device, &fence_info, nullptr, &frame_fences[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create semaphore");
        }
    }

    for (size_t i = 0; i < swap_chain_image_count; ++i)
    {
        if (vkCreateSemaphore(device, &semaphore_info, nullptr, &submit_semaphores[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create semaphore");
        }
    }
}
}