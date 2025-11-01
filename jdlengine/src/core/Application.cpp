#include "core/Application.hpp"

#include "utils/Logger.hpp"


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
}

void Application::run() {}

} // namespace core
} // namespace jdl
