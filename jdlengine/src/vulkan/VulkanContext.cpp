#include "vulkan/VulkanContext.hpp"

#include "core/Application.hpp"
#include "core/Window.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace vk
{

VulkanContext VulkanContext::CONTEXT;

void VulkanContext::doInit()
{
    if (m_instance != VK_NULL_HANDLE) {
        return;
    }

    createInstance();
}

void VulkanContext::doDestroy()
{
    if (m_instance == VK_NULL_HANDLE) {
        return;
    }

    vkDestroyInstance(m_instance, nullptr);
    m_instance = VK_NULL_HANDLE;
}

void VulkanContext::createInstance()
{
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = core::Application::GetName().c_str();
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "JDLEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    std::vector<const char*> extensions = core::Window::GetInstanceExtensions();

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();
    createInfo.enabledLayerCount = 0;

    VK_CALL(vkCreateInstance(&createInfo, nullptr, &m_instance));
}

} // namespace vk
} // namespace jdl
