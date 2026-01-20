#include "vulkan/framebuffers.hpp"

#include <stdexcept>
#include <iostream>

namespace niqqa
{
Framebuffers::Framebuffers(VkDevice device,
                           VkRenderPass render_pass,
                           VkExtent2D extent,
                           const std::vector<VkImageView> &image_views)
    : m_device(device)
{
    init_framebuffers(render_pass, extent, image_views);
}

Framebuffers::~Framebuffers()
{
    cleanup();
}

void Framebuffers::cleanup() noexcept
{
    for (auto framebuffer : m_framebuffers)
    {
        vkDestroyFramebuffer(m_device, framebuffer, nullptr);
    }
}

void Framebuffers::recreate(VkRenderPass render_pass, VkExtent2D extent, const std::vector<VkImageView> &image_views) noexcept
{
    init_framebuffers(render_pass, extent, image_views);
}

const std::vector<VkFramebuffer> &Framebuffers::get_framebuffers() const noexcept
{
    return m_framebuffers;
}

void Framebuffers::init_framebuffers(VkRenderPass render_pass, VkExtent2D extent, const std::vector<VkImageView> &image_views)
{
    m_framebuffers.resize(image_views.size());

    for (size_t i = 0; i < image_views.size(); ++i)
    {
        VkFramebufferCreateInfo framebuffer_info{};
        framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebuffer_info.renderPass = render_pass;
        framebuffer_info.attachmentCount = 1;
        framebuffer_info.pAttachments = &image_views[i];
        framebuffer_info.width = extent.width;
        framebuffer_info.height = extent.height;
        framebuffer_info.layers = 1;

        if (vkCreateFramebuffer(m_device, &framebuffer_info, nullptr, &m_framebuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create framebuffer");
        }
    }
}
}