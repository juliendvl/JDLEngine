#pragma once

#include "Core.hpp"

#include "utils/NonCopyable.hpp"


namespace jdl
{
namespace core
{

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

private:
    static VulkanContext CONTEXT;

    void doInit();
    void doDestroy();

    // Vulkan instance
    VkInstance m_instance = VK_NULL_HANDLE;
    // Debug messenger
    VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;

    void createInstance();
    void createDebugMessenger();
};

} // namespace core
} // namespace jdl
