#include "core/Window.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace core
{

Window* Window::IWindow = nullptr;

Window::Window(const char* title, int width, int height)
{
    if (IWindow != nullptr) {
        JDL_FATAL("The window has already been created");
    }
    IWindow = this;

    glfwSetErrorCallback(
        [](int code, const char* description) { JDL_ERROR("GLFW {}: {}", code, description); }
    );

    int status = glfwInit();
    if (status == GLFW_FALSE) {
        JDL_FATAL("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (m_window == nullptr) {
        JDL_FATAL("Failed to create the GLFW window");
    }
    glfwSetWindowUserPointer(m_window, this);
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

Size Window::getSize() const
{
    Size size;
    glfwGetWindowSize(m_window, &size.width, &size.height);

    return size;
}

Size Window::getFramebufferSize() const
{
    Size size;
    glfwGetFramebufferSize(m_window, &size.width, &size.height);
    
    return size;
}

bool Window::isClosing() const
{
    int status = glfwWindowShouldClose(m_window);
    return status == GLFW_TRUE;
}

std::vector<const char*> Window::GetRequiredInstanceExtensions()
{
    uint32_t nbExtensions;
    const char** extensions = glfwGetRequiredInstanceExtensions(&nbExtensions);

    return std::vector<const char*>(extensions, extensions + nbExtensions);
}

} // namespace core
} // namespace jdl
