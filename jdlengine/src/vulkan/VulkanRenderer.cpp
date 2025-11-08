#include "vulkan/VulkanRenderer.hpp"
#include "vulkan/VulkanContext.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace vk
{

VulkanRenderer::VulkanRenderer()
{
    // Init the VulkanContext
    VulkanContext::Init();
}

VulkanRenderer::~VulkanRenderer()
{
    // Destroy the VulkanContext
    VulkanContext::Destroy();
}

void VulkanRenderer::renderFrame() {}

} // namespace vk
} // namespace jdl
