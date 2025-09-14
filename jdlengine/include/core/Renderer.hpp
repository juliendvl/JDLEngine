#pragma once

#include "Core.hpp"

#include "utils/NonCopyable.hpp"


namespace jdl
{
namespace core
{

class JDL_API Renderer : private NonCopyable<Renderer>
{
public:
    /**
     * @brief Creates the renderer.
     */
    Renderer();

    /**
     * @brief Destroys the renderer.
     */
    ~Renderer();

    /**
     * @brief Renders a frame.
     */
    void renderFrame();

private:
    // Command buffers
    std::vector<VkCommandBuffer> m_commandBuffers;
    // Synchronization objects
    std::vector<VkSemaphore> m_imageSemaphores;
    std::vector<VkSemaphore> m_renderSemaphores;
    std::vector<VkFence> m_startFences;

    // Background color
    VkClearValue m_backgroundColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};

    // Current frame index
    uint32_t m_currentFrame = 0;

    void createSynchronizationObjects();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
};

} // namespace core
} // namespace jdl
