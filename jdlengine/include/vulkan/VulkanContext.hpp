#pragma once

#include "core/Core.hpp"

#include "utils/NonCopyable.hpp"


namespace jdl
{
namespace vk
{

class JDL_API VulkanContext : private NonCopyable<VulkanContext>
{
public:
    /**
     * @brief Inits the Vulkan context.
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
     * @brief Returns the Vulkan instance object.
     */
    static VkInstance GetInstance() {
        return CONTEXT.m_instance;
    }

private:
    static VulkanContext CONTEXT;

    // Vulkan instance
    VkInstance m_instance = VK_NULL_HANDLE;

    void doInit();
    void doDestroy();

    void createInstance();
};

} // namespace vk
} // namespace jdl

