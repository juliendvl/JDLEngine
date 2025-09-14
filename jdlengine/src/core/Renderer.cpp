#include "core/Renderer.hpp"
#include "core/VulkanContext.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace core
{

static const uint32_t sFramesInFlight = 2;

Renderer::Renderer()
{
    m_commandBuffers = VulkanContext::AllocateCommandBuffers(sFramesInFlight);
    createSynchronizationObjects();
}

Renderer::~Renderer()
{
    VkDevice device = VulkanContext::GetDevice();

    vkDeviceWaitIdle(device);
    for (auto i = 0; i < sFramesInFlight; ++i)
    {
        vkDestroySemaphore(device, m_imageSemaphores[i], nullptr);
        vkDestroySemaphore(device, m_renderSemaphores[i], nullptr);
        vkDestroyFence(device, m_startFences[i], nullptr);
    }
}

void Renderer::renderFrame()
{
    VkDevice device = VulkanContext::GetDevice();

    VkCommandBuffer commandBuffer = m_commandBuffers[m_currentFrame];
    VkFence startFence = m_startFences[m_currentFrame];
    VkSemaphore imageSemaphore = m_imageSemaphores[m_currentFrame];
    VkSemaphore renderSemaphore = m_renderSemaphores[m_currentFrame];
    VkSwapchainKHR swapChain = VulkanContext::GetSwapChain().getHandle();

    vkWaitForFences(device, 1, &startFence, VK_TRUE, UINT64_MAX);
    vkResetFences(device, 1, &startFence);

    // Retrieve the swap chain next image
    uint32_t imageIndex = VulkanContext::GetSwapChain().getNextImageIndex(imageSemaphore);
    recordCommandBuffer(commandBuffer, imageIndex);

    // Submit the command buffer
    VkPipelineStageFlags waitStage = {VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT};

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &imageSemaphore;
    submitInfo.pWaitDstStageMask = &waitStage;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &renderSemaphore;

    VK_CALL(vkQueueSubmit(VulkanContext::GetGraphicsQueue(), 1, &submitInfo, startFence));

    // Present the image to the swap chain
    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &renderSemaphore;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapChain;
    presentInfo.pImageIndices = &imageIndex;

    VK_CALL(vkQueuePresentKHR(VulkanContext::GetPresentQueue(), &presentInfo));

    m_currentFrame = (m_currentFrame + 1) % sFramesInFlight;
}

void Renderer::createSynchronizationObjects()
{
    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    m_imageSemaphores.resize(sFramesInFlight);
    m_renderSemaphores.resize(sFramesInFlight);
    m_startFences.resize(sFramesInFlight);

    VkDevice device = VulkanContext::GetDevice();
    for (auto i = 0; i < sFramesInFlight; ++i)
    {
        VK_CALL(vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_imageSemaphores[i]));
        VK_CALL(vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_renderSemaphores[i]));
        VK_CALL(vkCreateFence(device, &fenceInfo, nullptr, &m_startFences[i]));
    }
}

void Renderer::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    VK_CALL(vkResetCommandBuffer(commandBuffer, 0));
    VK_CALL(vkBeginCommandBuffer(commandBuffer, &beginInfo));
    {
        VkExtent2D extent = VulkanContext::GetSwapChain().getExtent();

        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = VulkanContext::GetRenderPass().getHandle();
        renderPassInfo.framebuffer = VulkanContext::GetSwapChain().getFramebuffer(imageIndex);
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = extent;
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &m_backgroundColor;

        VkViewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(extent.width);
        viewport.height = static_cast<float>(extent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor = {};
        scissor.offset = {0, 0};
        scissor.extent = extent;

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanContext::GetPipeline().getHandle());
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
        vkCmdDraw(commandBuffer, 3, 1, 0, 0);
        vkCmdEndRenderPass(commandBuffer);
    }
    VK_CALL(vkEndCommandBuffer(commandBuffer));
}

} // namespace core
} // namespace jdl
