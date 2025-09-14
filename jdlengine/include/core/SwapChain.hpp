#pragma once

#include "Core.hpp"

#include "utils/NonCopyable.hpp"


namespace jdl
{
namespace core
{

class JDL_API SwapChain : private NonCopyable<SwapChain>
{
public:
    /**
     * @brief Creates the swap chain.
     */
    SwapChain();

    /**
     * @brief Destroys the swap chain.
     */
    ~SwapChain();

    /**
     * @brief Returns the selected surface format.
     */
    VkSurfaceFormatKHR getSurfaceFormat() const {
        return m_surfaceFormat;
    }

    /**
     * @brief Returns the selected extent.
     */
    VkExtent2D getExtent() const {
        return m_extent;
    }

    /**
     * @brief Returns the swap chain Vulkan handle.
     */
    VkSwapchainKHR getHandle() const {
        return m_swapChain;
    }

    /**
     * @brief Returns the number of images in this swap chain.
     */
    size_t getNbImages() const {
        return m_images.size();
    }

    /**
     * @brief  Returns a swap chain image.
     * @param  index Image index. Must be < getNbImages().
     * @return Queried image, or VK_NULL_HANDLE if the index is invalid
     */
    VkImage getImage(size_t index) const {
        return index < getNbImages() ? m_images[index] : VK_NULL_HANDLE;
    }

    /**
     * @brief  Returns a swap chain image view.
     * @param  index Image view index. Must be < getNbImages().
     * @return Queried image view, or VK_NULL_HANDLE if the index is invalid
     */
    VkImageView getImageView(size_t index) const {
        return index < getNbImages() ? m_views[index] : VK_NULL_HANDLE;
    }

    /**
     * @brief Creates the framebuffers. Needs the render pass to be created.
     */
    void createFramebuffers();

    /**
     * @brief  Returns a swap chain framebuffer.
     * @param  index Framebuffer index. Must be < getNbImages().
     * @return Framebuffer, or VK_NULL_HANDLE if the index is invalid.
     */
    VkFramebuffer getFramebuffer(size_t index) const {
        return index < m_framebuffers.size() ? m_framebuffers[index] : VK_NULL_HANDLE;
    }

    /**
     * @brief Returns the next available frame index.
     * @param semaphore Optional signal semaphore
     * @param fence Optional signal fence
     */
    uint32_t getNextImageIndex(VkSemaphore semaphore = VK_NULL_HANDLE, VkFence fence = VK_NULL_HANDLE) const;

private:
    // Surface format
    VkSurfaceFormatKHR m_surfaceFormat = {};
    // Presentation mode
    VkPresentModeKHR m_presentMode = VK_PRESENT_MODE_FIFO_KHR;
    // Extent
    VkExtent2D m_extent = {};
    // Vulkan handle
    VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;

    std::vector<VkImage> m_images;
    std::vector<VkImageView> m_views;
    std::vector<VkFramebuffer> m_framebuffers;

    void create();
    void createViews();
};

} // namespace core
} // namespace jdl
