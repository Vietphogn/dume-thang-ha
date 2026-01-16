#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <cstdint>

namespace niqqa
{
class SyncObjects
{
public:
    SyncObjects(VkDevice device, uint32_t frames_in_flight, uint32_t swap_chain_image_count);
    ~SyncObjects();

    VkSemaphore acquire_semaphore(uint32_t frame_index) const noexcept;
    VkFence frame_fence(uint32_t frame_index) const noexcept;
    VkSemaphore submit_semaphore(uint32_t image_index) const noexcept;

private:
    VkDevice device;

    std::vector<VkSemaphore> acquire_semaphores;
    std::vector<VkSemaphore> submit_semaphores;
    std::vector<VkFence> frame_fences;

    void init_sync_objects(uint32_t frames_in_flight, uint32_t image_count);
};
}