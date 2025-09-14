#include "core/RenderPass.hpp"
#include "core/VulkanContext.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace core
{

RenderPass::RenderPass()
{
    auto& swapChain = VulkanContext::GetSwapChain();

    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = swapChain.getSurfaceFormat().format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentReference = {};
    colorAttachmentReference.attachment = 0;
    colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentReference;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    VkDevice device = VulkanContext::GetDevice();
    VK_CALL(vkCreateRenderPass(device, &renderPassInfo, nullptr, &m_renderPass));
}

RenderPass::~RenderPass()
{
    VkDevice device = VulkanContext::GetDevice();
    vkDestroyRenderPass(device, m_renderPass, nullptr);
}

} // namespace core
} // namespace jdl
