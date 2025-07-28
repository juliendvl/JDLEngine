#include "core/Application.hpp"
#include "core/VulkanContext.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace core
{

Application* Application::IApplication = nullptr;

Application::Application(const char* name, int width, int height)
    : m_name(name)
{
    if (IApplication != nullptr)
    {
        JDL_FATAL("The Application has already been created");
    }
    IApplication = this;

    JDL_INFO("Creating the application window");
    m_window = std::make_unique<Window>(name, width, height);

    VulkanContext::Init();
}

Application::~Application()
{
    VulkanContext::Destroy();

    JDL_INFO("Destroying the application window");
    m_window.reset();
}

void Application::run()
{
    while (!m_window->isClosing())
    {
        m_window->pollEvents();
    }
}

} // namespace core
} // namespace jdl
