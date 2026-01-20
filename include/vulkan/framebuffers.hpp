#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace niqqa
{
class Framebuffers
{
public:
    Framebuffers(VkDevice device,
                 VkRenderPass render_pass,
                 VkExtent2D extent,
                 const std::vector<VkImageView> &image_views);
    ~Framebuffers();

    void cleanup() noexcept;
    void recreate(VkRenderPass render_pass, VkExtent2D extent, const std::vector<VkImageView> &image_views) noexcept;

    const std::vector<VkFramebuffer> &get_framebuffers() const noexcept;

private:
    VkDevice m_device{VK_NULL_HANDLE};
    std::vector<VkFramebuffer> m_framebuffers{VK_NULL_HANDLE};

    void init_framebuffers(VkRenderPass render_pass, VkExtent2D extent, const std::vector<VkImageView> &image_views);
};
}