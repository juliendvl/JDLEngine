#pragma once

#include "Core.hpp"

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

    /**
     * @brief Returns whether all the required queue family indices have been set or not.
     */
    bool isComplete() const {
        return graphics.has_value();
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

private:
    static VulkanContext CONTEXT;

    void doInit();
    void doDestroy();

    // Vulkan instance
    VkInstance m_instance = VK_NULL_HANDLE;
    // Debug messenger
    VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;
    // Selected physical device
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    // Logical device
    VkDevice m_device = VK_NULL_HANDLE;
    // Queues
    VkQueue m_graphicsQueue = VK_NULL_HANDLE;

    // Queue family indices for the selected physical device
    QueueFamilyIndices m_queueFamilyIndices;

    void createInstance();
    void createDebugMessenger();
    void selectPhysicalDevice();
    void createDevice();
};

} // namespace core
} // namespace jdl
