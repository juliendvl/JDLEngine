#include "core/VulkanContext.hpp"
#include "core/Application.hpp"
#include "core/Window.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace core
{

/* --- Validation Layers --- */
#ifndef NDEBUG
static const bool sUseValidationLayers = true;
#else
static const bool sUseValidationLayers = false;
#endif

static const std::vector<const char*> sValidationLayers { "VK_LAYER_KHRONOS_validation" };

static void s_CheckValidationLayersSupport()
{
    uint32_t nbLayers;
    VK_CALL(vkEnumerateInstanceLayerProperties(&nbLayers, nullptr));

    std::vector<VkLayerProperties> layers(nbLayers);
    VK_CALL(vkEnumerateInstanceLayerProperties(&nbLayers, layers.data()));

    for (const char* requiredLayer : sValidationLayers)
    {
        bool layerFound = false;
        for (const auto& layer : layers)
        {
            if (strcmp(layer.layerName, requiredLayer) == 0)
            {
                layerFound = true;
                break;
            }
        }
        if (!layerFound){
            JDL_FATAL("Validation layer {} is not supported", requiredLayer);
        }
    }
}

static VKAPI_ATTR VkBool32 VKAPI_CALL s_DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT* data,
    void* userData
)
{
    switch (severity)
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            JDL_TRACE(data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            JDL_INFO(data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            JDL_WARN(data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            JDL_ERROR(data->pMessage);
            break;
        default:
            break;
    }

    return VK_FALSE;
}

static VkDebugUtilsMessengerCreateInfoEXT s_GetDebugMessengerCreateInfo()
{
    VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = s_DebugCallback;

    return createInfo;
}

static VkResult s_CreateDebugMessenger(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger
)
{
    auto fn = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (fn != nullptr) {
        return fn(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

static void s_DestroyDebugMessenger(
    VkInstance instance,
    VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks* pAllocator
)
{
    auto fn = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (fn != nullptr) {
        fn(instance, debugMessenger, pAllocator);
    }
}


/* --- VulkanContext CLASS --- */
VulkanContext VulkanContext::CONTEXT;

void VulkanContext::doInit()
{
    if (m_instance == VK_NULL_HANDLE)
    {
        JDL_INFO("VulkanContext - Creating instance");
        createInstance();

        if (sUseValidationLayers)
        {
            JDL_INFO("VulkanContext - Creating debug messenger");
            createDebugMessenger();
        }
    }
}

void VulkanContext::doDestroy()
{
    if (m_instance != VK_NULL_HANDLE)
    {
        if (m_debugMessenger != VK_NULL_HANDLE)
        {
            JDL_INFO("VulkanContext - Destroying debug messenger");
            s_DestroyDebugMessenger(m_instance, m_debugMessenger, nullptr);
        }

        JDL_INFO("VulkanContext - Destroying instance");
        vkDestroyInstance(m_instance, nullptr);

        m_instance = VK_NULL_HANDLE;
    }
}

void VulkanContext::createInstance()
{
    if (sUseValidationLayers) {
        s_CheckValidationLayersSupport();
    }
    VkDebugUtilsMessengerCreateInfoEXT debugMessengerInfo = s_GetDebugMessengerCreateInfo();

    std::vector<const char*> extensions = Window::GetRequiredInstanceExtensions();
    if (sUseValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = Application::GetName().c_str();
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "JDLEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_2;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    if (sUseValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(sValidationLayers.size());
        createInfo.ppEnabledLayerNames = sValidationLayers.data();
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugMessengerInfo;
    }
    else {
        createInfo.enabledLayerCount = 0;
    }

    VK_CALL(vkCreateInstance(&createInfo, nullptr, &m_instance));
}

void VulkanContext::createDebugMessenger()
{
    VkDebugUtilsMessengerCreateInfoEXT createInfo = s_GetDebugMessengerCreateInfo();
    VK_CALL(s_CreateDebugMessenger(m_instance, &createInfo, nullptr, &m_debugMessenger));
}

} // namespace core
} // namespace jdl
