#pragma once

#include "platform/window.hpp"
#include "vulkan/swap_chain.hpp"
#include "vulkan/render_pass.hpp"
#include "vulkan/pipeline.hpp"
#include "vulkan/command_buffers.hpp"
#include "vulkan/device.hpp"
#include "vulkan/framebuffers.hpp"
#include "vulkan/sync_objects.hpp"

namespace niqqa
{
class Renderer
{
public: 
    Renderer(VkExtent2D extent, VkSurfaceKHR surface, VkInstance instance);
    ~Renderer();

    void draw_frame();
    void wait_idle() noexcept;

private:
    static constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;
    uint32_t current_frame = 0;

    VkSurfaceKHR surface;
    Device device;
    SwapChain swap_chain;
    SyncObjects sync_objects;
    RenderPass render_pass;
    GraphicsPipeline graphics_pipeline;    
    Framebuffers framebuffers;
    CommandBuffers command_buffers;

    void cleanup_swap_chain();
    void recreate_swap_chain();
};
}