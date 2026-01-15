#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace niqqa
{
class Framebuffers
{
public:
    Framebuffers(VkDevice _device,
                 VkRenderPass render_pass,
                 VkExtent2D extent,
                 const std::vector<VkImageView> &image_views);
    ~Framebuffers();

private:
    VkDevice device;
    std::vector<VkFramebuffer> framebuffers;

    void init_framebuffers(VkRenderPass render_pass, VkExtent2D extent, const std::vector<VkImageView> &image_views);
};
}