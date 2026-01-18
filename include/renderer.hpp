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
    static constexpr uint32_t s_max_frames_in_flight{2};
    uint32_t m_current_frame{0};

    VkSurfaceKHR m_surface;
    Device m_device;
    SwapChain m_swap_chain;
    SyncObjects m_sync_objects;
    RenderPass m_render_pass;
    GraphicsPipeline m_graphics_pipeline;    
    Framebuffers m_framebuffers;
    CommandBuffers m_command_buffers;

    void cleanup_swap_chain() noexcept;
    void recreate_swap_chain();
};
}