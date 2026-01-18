#pragma once

#include <vulkan/vulkan.h>

namespace niqqa
{
class RenderPass
{
public:
    RenderPass(VkDevice device, VkFormat image_format);
    ~RenderPass();

    VkRenderPass get_render_pass() const noexcept;

private:
    VkRenderPass m_render_pass{VK_NULL_HANDLE};
    VkDevice m_device{VK_NULL_HANDLE};

    void init_render_pass(VkFormat image_format);
};
}