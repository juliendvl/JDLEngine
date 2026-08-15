#pragma once

#include "vk/vulkan_command_buffer.hpp"


namespace jdl
{
namespace core
{

struct RenderContext
{
	// Current command buffer
	vk::VulkanCommandBuffer* command_buffer = nullptr;
};

} // namespace core
} // namespace jdl
