#include "core/Application.hpp"

#include "utils/Logger.hpp"

#include "vulkan/VulkanContext.hpp"


namespace jdl
{
namespace core
{

Application* Application::IApplication = nullptr;

Application::Application(const std::string& name, size_t width, size_t height)
    : m_name(name)
{
    if (IApplication != nullptr) {
        JDL_FATAL("The application has already been created");
    }
    IApplication = this;

    // Create the application window
    m_window = std::make_unique<Window>(name, width, height);
    // Initialize the Vulkan context
    vk::VulkanContext::Init();
}

Application::~Application()
{
    // Destroy all the Vulkan resources
    vk::VulkanContext::Destroy();
    // Destroy the application window
    m_window.reset();
}

void Application::run()
{
    while (m_window->isRunning())
    {
        m_window->pollEvents();
    }
}

} // namespace core
} // namespace jdl
