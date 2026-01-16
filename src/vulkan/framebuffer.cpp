#include "vulkan/framebuffers.hpp"

#include <stdexcept>
#include <iostream>

namespace niqqa
{
Framebuffers::Framebuffers(VkDevice _device,
                           VkRenderPass render_pass,
                           VkExtent2D extent,
                           const std::vector<VkImageView> &image_views)
    : device(_device)
{
    init_framebuffers(render_pass, extent, image_views);
}

Framebuffers::~Framebuffers()
{
    for (auto framebuffer : framebuffers)
    {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }

    //std::cout << "~Framebuffer()\n";
}

void Framebuffers::cleanup()
{
    for (auto framebuffer : framebuffers)
    {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }
}

const std::vector<VkFramebuffer> &Framebuffers::get_framebuffers() const noexcept
{
    return framebuffers;
}

void Framebuffers::init_framebuffers(VkRenderPass render_pass, VkExtent2D extent, const std::vector<VkImageView> &image_views)
{
    framebuffers.resize(image_views.size());

    for (size_t i = 0; i < image_views.size(); ++i)
    {
        VkImageView attachments[] = {
            image_views[i]
        };

        VkFramebufferCreateInfo framebuffer_info{};
        framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebuffer_info.renderPass = render_pass;
        framebuffer_info.attachmentCount = 1;
        framebuffer_info.pAttachments = attachments;
        framebuffer_info.width = extent.width;
        framebuffer_info.height = extent.height;
        framebuffer_info.layers = 1;

        if (vkCreateFramebuffer(device, &framebuffer_info, nullptr, &framebuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create framebuffer");
        }
    }
}
}