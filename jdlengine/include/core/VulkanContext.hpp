#pragma once

#include "Core.hpp"
#include "Pipeline.hpp"
#include "RenderPass.hpp"
#include "SwapChain.hpp"

#include "utils/NonCopyable.hpp"

#include <optional>
#include <set>


namespace jdl
{
namespace core
{

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphics;
    std::optional<uint32_t> present;

    /**
     * @brief Returns whether all the required queue family indices have been set or not.
     */
    bool isComplete() const {
        return graphics.has_value() && present.has_value();
    }

    /**
     * @brief Returns the unique queue family indices.
     */
    std::set<uint32_t> getUniqueIndices() const
    {
        std::set<uint32_t> indices;
        if (isComplete())
        {
            indices.insert(*graphics);
            indices.insert(*present);
        }
        return indices;
    }
};

class JDL_API VulkanContext : private NonCopyable<VulkanContext>
{
public:
    /**
     * @brief Initializes the Vulkan context.
     */
    static void Init() {
        CONTEXT.doInit();
    }

    /**
     * @brief Destroys the Vulkan context.
     */
    static void Destroy() {
        CONTEXT.doDestroy();
    }

    /**
     * @brief Returns the Vulkan instance.
     */
    static VkInstance GetInstance() {
        return CONTEXT.m_instance;
    }

    /**
     * @brief Returns the Vulkan window surface.
     */
    static VkSurfaceKHR GetWindowSurface() {
        return CONTEXT.m_windowSurface;
    }

    /**
     * @brief Returns the selected physical device.
     */
    static VkPhysicalDevice GetPhysicalDevice() {
        return CONTEXT.m_physicalDevice;
    }

    /**
     * @brief Returns the queue family indices for the selected physical device.
     */
    static const QueueFamilyIndices& GetQueueFamilyIndices() {
        return CONTEXT.m_queueFamilyIndices;
    }

    /**
     * @brief Returns the device handle.
     */
    static VkDevice GetDevice() {
        return CONTEXT.m_device;
    }

    /**
     * @brief Returns the graphics queue handle.
     */
    static VkQueue GetGraphicsQueue() {
        return CONTEXT.m_graphicsQueue;
    }

    /**
     * @brief Returns the present queue handle.
     */
    static VkQueue GetPresentQueue() {
        return CONTEXT.m_presentQueue;
    }

    /**
     * @brief Returns the current swap chain object.
     */
    static SwapChain& GetSwapChain() {
        return *CONTEXT.m_swapChain;
    }

    /**
     * @brief Returns the render pass object.
     */
    static RenderPass& GetRenderPass() {
        return *CONTEXT.m_renderPass;
    }

    /**
     * @brief Returns the current pipeline object.
     */
    static Pipeline& GetPipeline() {
        return *CONTEXT.m_pipeline;
    }

    /**
     * @brief Allocates and returns command buffers.
     * @param count The number of command buffers to allocate
     */
    static std::vector<VkCommandBuffer> AllocateCommandBuffers(size_t count = 1);

private:
    static VulkanContext CONTEXT;

    void doInit();
    void doDestroy();

    // Vulkan instance
    VkInstance m_instance = VK_NULL_HANDLE;
    // Debug messenger
    VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;
    // Window surface
    VkSurfaceKHR m_windowSurface = VK_NULL_HANDLE;
    // Selected physical device
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    // Logical device
    VkDevice m_device = VK_NULL_HANDLE;
    // Queues
    VkQueue m_graphicsQueue = VK_NULL_HANDLE;
    VkQueue m_presentQueue = VK_NULL_HANDLE;
    // Command pool
    VkCommandPool m_commandPool = VK_NULL_HANDLE;

    // Queue family indices for the selected physical device
    QueueFamilyIndices m_queueFamilyIndices;

    std::unique_ptr<SwapChain> m_swapChain;
    std::unique_ptr<RenderPass> m_renderPass;
    std::unique_ptr<Pipeline> m_pipeline;

    void createInstance();
    void createDebugMessenger();
    void createWindowSurface();
    void selectPhysicalDevice();
    void createDevice();
    void createDefaultResources();
    void createSwapChain();
    void createRenderPass();
    void createPipeline();
};

} // namespace core
} // namespace jdl
