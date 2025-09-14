#include "core/VulkanContext.hpp"
#include "core/Application.hpp"
#include "core/Window.hpp"

#include "resource/ResourceManager.hpp"
#include "resource/Shader.hpp"

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


/* --- Device Extensions --- */
static const std::vector<const char*> sDeviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

static bool s_DeviceExtensionsSupported(VkPhysicalDevice device)
{
    uint32_t nbExtensions = 0;
    VK_CALL(vkEnumerateDeviceExtensionProperties(device, nullptr, &nbExtensions, nullptr));

    std::vector<VkExtensionProperties> extensions(nbExtensions);
    VK_CALL(vkEnumerateDeviceExtensionProperties(device, nullptr, &nbExtensions, extensions.data()));

    std::set<std::string> requiredExtensions(sDeviceExtensions.begin(), sDeviceExtensions.end());
    for (const auto& extension : extensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}


/* --- VulkanContext CLASS --- */
VulkanContext VulkanContext::CONTEXT;

void VulkanContext::doInit()
{
    if (m_instance == VK_NULL_HANDLE)
    {
        createInstance();

        if (sUseValidationLayers) {
            createDebugMessenger();
        }

        createWindowSurface();
        selectPhysicalDevice();
        createDevice();
        createDefaultResources();
        createSwapChain();
        createRenderPass();
        createPipeline();
    }
}

void VulkanContext::doDestroy()
{
    if (m_instance != VK_NULL_HANDLE)
    {
        JDL_INFO("VulkanContext - Destroying graphics pipeline");
        m_pipeline.reset();

        JDL_INFO("VulkanContext - Destroying render pass");
        m_renderPass.reset();

        JDL_INFO("VulkanContext - Destroying swap chain");
        m_swapChain.reset();

        JDL_INFO("VulkanContext - Destroying device");
        vkDestroyDevice(m_device, nullptr);

        JDL_INFO("VulkanContext - Creating window surface");
        vkDestroySurfaceKHR(m_instance, m_windowSurface, nullptr);

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

    JDL_INFO("VulkanContext - Creating instance");
    VK_CALL(vkCreateInstance(&createInfo, nullptr, &m_instance));
}

void VulkanContext::createDebugMessenger()
{
    VkDebugUtilsMessengerCreateInfoEXT createInfo = s_GetDebugMessengerCreateInfo();

    JDL_INFO("VulkanContext - Creating debug messenger");
    VK_CALL(s_CreateDebugMessenger(m_instance, &createInfo, nullptr, &m_debugMessenger));
}

void VulkanContext::createWindowSurface()
{
    JDL_INFO("VulkanContext - Creating window surface");
    m_windowSurface = Window::Get().createWindowSurface();
}

void VulkanContext::selectPhysicalDevice()
{
    uint32_t nbDevices;
    VK_CALL(vkEnumeratePhysicalDevices(m_instance, &nbDevices, nullptr));

    std::vector<VkPhysicalDevice> physicalDevices(nbDevices);
    if (nbDevices == 0) {
        JDL_FATAL("Cannot find GPUs with Vulkan support");
    }
    VK_CALL(vkEnumeratePhysicalDevices(m_instance, &nbDevices, physicalDevices.data()));

    std::vector<VkPhysicalDevice> validDevices;
    std::vector<QueueFamilyIndices> validQueueFamilyIndices;

    for (VkPhysicalDevice device : physicalDevices)
    {
        if (!s_DeviceExtensionsSupported(device)) {
            continue;
        }

        uint32_t nbQueues = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &nbQueues, nullptr);

        if (nbQueues == 0) {
            continue;
        }
        std::vector<VkQueueFamilyProperties> queues(nbQueues);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &nbQueues, queues.data());

        QueueFamilyIndices queueIndices;
        for (uint32_t i = 0; i < nbQueues; ++i)
        {
            // Graphics support
            if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                queueIndices.graphics = i;
            }
            // Present support
            VkBool32 presentSupported = VK_FALSE;
            VK_CALL(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_windowSurface, &presentSupported));
            if (presentSupported) {
                queueIndices.present = i;
            }

            if (queueIndices.isComplete())
            {
                validDevices.push_back(device);
                validQueueFamilyIndices.push_back(queueIndices);
                break;
            }
        }
    }
    if (validDevices.empty()) {
        JDL_FATAL("Cannot find GPUs compatible with the application requirements");
    }

    // When multiple physical devices are valid, select in priority the dedicated GPU if any
    m_physicalDevice = validDevices[0];
    m_queueFamilyIndices = validQueueFamilyIndices[0];

    for (auto i = 1; i < validDevices.size(); ++i)
    {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(validDevices[i], &deviceProperties);

        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            m_physicalDevice = validDevices[i];
            m_queueFamilyIndices = validQueueFamilyIndices[i];
            break;
        }
    }

    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(m_physicalDevice, &deviceProperties);
    JDL_INFO("VulkanContext - Selected physical device: {}", deviceProperties.deviceName);
}

void VulkanContext::createDevice()
{
    float priority = 1.0f;

    // Device queues
    std::vector<VkDeviceQueueCreateInfo> queuesCreateInfo;
    for (uint32_t queueIndex : m_queueFamilyIndices.getUniqueIndices())
    {
        VkDeviceQueueCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        createInfo.queueFamilyIndex = queueIndex;
        createInfo.queueCount = 1;
        createInfo.pQueuePriorities = &priority;

        queuesCreateInfo.push_back(createInfo);
    }

    // Device features
    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queuesCreateInfo.size());
    createInfo.pQueueCreateInfos = queuesCreateInfo.data();
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(sDeviceExtensions.size());
    createInfo.ppEnabledExtensionNames = sDeviceExtensions.data();
    createInfo.enabledLayerCount = 0;

    JDL_INFO("VulkanContext - Creating device");
    VK_CALL(vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device));

    // Retrieve queues handles
    vkGetDeviceQueue(m_device, *m_queueFamilyIndices.graphics, 0, &m_graphicsQueue);
    vkGetDeviceQueue(m_device, *m_queueFamilyIndices.present, 0, &m_presentQueue);
}

void VulkanContext::createDefaultResources()
{
    resource::ResourceManager::Create<resource::Shader>("default_vert", "shaders/default.vert.spv");
    resource::ResourceManager::Create<resource::Shader>("default_frag", "shaders/default.frag.spv");
}

void VulkanContext::createSwapChain()
{
    JDL_INFO("VulkanContext - Creating swap chain");
    m_swapChain = std::make_unique<SwapChain>();
}

void VulkanContext::createRenderPass()
{
    JDL_INFO("VulkanContext - Creating render pass");
    m_renderPass = std::make_unique<RenderPass>();

    m_swapChain->createFramebuffers();
}

void VulkanContext::createPipeline()
{
    JDL_INFO("VulkanContext - Creating graphics pipeline");

    m_pipeline = std::make_unique<Pipeline>();
    m_pipeline->addShader("default_vert", ShaderStage::eVertex);
    m_pipeline->addShader("default_frag", ShaderStage::eFragment);
    m_pipeline->create();
}

} // namespace core
} // namespace jdl
