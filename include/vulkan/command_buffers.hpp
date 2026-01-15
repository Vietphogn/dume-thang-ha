#pragma once

#include <vulkan/vulkan.h>

namespace niqqa
{
class CommandBuffers
{
public:

private:
    VkCommandBuffer command_buffer;

    void init_command_buffer();
};
}