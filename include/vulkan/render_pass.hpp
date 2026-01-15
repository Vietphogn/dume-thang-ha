#pragma once

#include <vulkan/vulkan.h>

namespace niqqa
{
class RenderPass
{
public:
    RenderPass(VkDevice _device, VkFormat image_format);
    ~RenderPass();

    VkRenderPass get() const noexcept;

private:
    VkRenderPass render_pass;
    VkDevice device;

    void init_render_pass(VkFormat image_format);
};
}