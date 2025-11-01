#include "core/Application.hpp"


namespace jdl
{
namespace core
{

Application* Application::IApplication = nullptr;

Application::Application(const std::string& name, size_t width, size_t height)
    : m_name(name)
{
    if (IApplication != nullptr) {
        // TODO - Raise a Fatal error
    }
    IApplication = this;
}

void Application::run() {}

} // namespace core
} // namespace jdl
