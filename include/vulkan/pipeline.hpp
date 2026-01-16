#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace niqqa
{
class GraphicsPipeline
{
public:
    GraphicsPipeline(VkDevice _device, VkExtent2D extent, VkRenderPass render_pass);
    ~GraphicsPipeline();

    VkPipeline get_graphics_pipeline() const noexcept;

private:
    VkDevice device;
    VkPipelineLayout pipeline_layout;
    VkPipeline graphics_pipeline;

    void init_graphics_pipeline(VkExtent2D extent, VkRenderPass render_pass);
    VkShaderModule create_shader_module(const std::vector<char> &code);
};
}