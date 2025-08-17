#include "core/SwapChain.hpp"
#include "core/VulkanContext.hpp"
#include "core/Window.hpp"

#include "utils/Logger.hpp"
#include "utils/VulkanUtils.hpp"

#include <algorithm>


namespace jdl
{
namespace core
{

SwapChain::SwapChain()
{
    create();
    createViews();
}

SwapChain::~SwapChain()
{
    VkDevice device = VulkanContext::GetDevice();

    for (VkImageView view : m_views) {
        vkDestroyImageView(device, view, nullptr);
    }
    vkDestroySwapchainKHR(device, m_swapChain, nullptr);
}

void SwapChain::create()
{
    VkPhysicalDevice physicalDevice = VulkanContext::GetPhysicalDevice();
    VkSurfaceKHR surface = VulkanContext::GetWindowSurface();

    VkSurfaceCapabilitiesKHR capabilities;
    VK_CALL(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities));

    // Surface Format
    uint32_t nbFormats = 0;
    VK_CALL(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &nbFormats, nullptr));

    std::vector<VkSurfaceFormatKHR> formats(nbFormats);
    VK_CALL(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &nbFormats, formats.data()));

    m_surfaceFormat = formats[0];
    for (auto i = 1; i < formats.size(); ++i)
    {
        VkSurfaceFormatKHR format = formats[i];
        if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            m_surfaceFormat = format;
            break;
        }
    }

    // Present mode
    uint32_t nbModes = 0;
    VK_CALL(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &nbModes, nullptr));

    std::vector<VkPresentModeKHR> modes(nbModes);
    VK_CALL(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &nbModes, modes.data()));

    for (auto i = 0; i < modes.size(); ++i)
    {
        if (modes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            m_presentMode = modes[i];
            break;
        }
    }

    // Extent
    if (capabilities.currentExtent.width != UINT32_MAX) {
        m_extent = capabilities.currentExtent;
    }
    else
    {
        Size framebufferSize = Window::Get().getFramebufferSize();
        m_extent.width = std::clamp(
            static_cast<uint32_t>(framebufferSize.width),
            capabilities.minImageExtent.width,
            capabilities.maxImageExtent.width
        );
        m_extent.height = std::clamp(
            static_cast<uint32_t>(framebufferSize.height),
            capabilities.minImageExtent.height,
            capabilities.maxImageExtent.height
        );
    }

    // Number of images
    uint32_t nbImages = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0 && nbImages > capabilities.maxImageCount) {
        nbImages = capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.clipped = VK_TRUE;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.imageFormat = m_surfaceFormat.format;
    createInfo.imageColorSpace = m_surfaceFormat.colorSpace;
    createInfo.imageExtent = m_extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.minImageCount = nbImages;
    createInfo.oldSwapchain = VK_NULL_HANDLE;
    createInfo.preTransform = capabilities.currentTransform;
    createInfo.presentMode = m_presentMode;
    createInfo.surface = surface;

    const QueueFamilyIndices& indices = VulkanContext::GetQueueFamilyIndices();
    uint32_t queueFamilyIndices[] = {*indices.graphics, *indices.present};

    if (queueFamilyIndices[0] != queueFamilyIndices[1])
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    VkDevice device = VulkanContext::GetDevice();
    VK_CALL(vkCreateSwapchainKHR(device, &createInfo, nullptr, &m_swapChain));

    // Retrieve the swap chain images
    m_images.resize(nbImages);
    VK_CALL(vkGetSwapchainImagesKHR(device, m_swapChain, &nbImages, m_images.data()));
}

void SwapChain::createViews()
{
    m_views.reserve(m_images.size());
    for (VkImage image : m_images)
    {
        auto view = utils::VulkanUtils::CreateImageView(image, m_surfaceFormat.format, VK_IMAGE_ASPECT_COLOR_BIT);
        m_views.push_back(view);
    }
}

} // namespace core
} // namespace jdl
