#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace niqqa
{
class GraphicsPipeline
{
public:
    GraphicsPipeline(VkDevice device, VkExtent2D extent, VkRenderPass render_pass);
    ~GraphicsPipeline();

    VkPipeline get_graphics_pipeline() const noexcept;

private:
    VkDevice m_device{VK_NULL_HANDLE};
    VkPipelineLayout m_pipeline_layout{VK_NULL_HANDLE};
    VkPipeline m_graphics_pipeline{VK_NULL_HANDLE};

    void init_graphics_pipeline(VkExtent2D extent, VkRenderPass render_pass);
    VkShaderModule create_shader_module(const std::vector<char> &code);
};
}