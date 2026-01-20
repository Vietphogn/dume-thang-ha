#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <cstdint>

namespace niqqa
{
class SyncObjects
{
public:
    SyncObjects(VkDevice device, uint32_t frames_in_flight, uint32_t image_count);
    ~SyncObjects();

    VkSemaphore acquire_semaphore(uint32_t frame_index) const noexcept;
    VkFence frame_fence(uint32_t frame_index) const noexcept;
    VkSemaphore submit_semaphore(uint32_t image_index) const noexcept;
    void recreate(uint32_t image_count);

private:
    VkDevice m_device{VK_NULL_HANDLE};

    std::vector<VkSemaphore> m_acquire_semaphores;
    std::vector<VkSemaphore> m_submit_semaphores;
    std::vector<VkFence> m_frame_fences;

    void init_sync_objects(uint32_t frames_in_flight, uint32_t image_count);
};
}