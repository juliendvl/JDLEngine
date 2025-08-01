#pragma once

#include "Core.hpp"
#include "Size.hpp"

#include "utils/NonCopyable.hpp"

#include <GLFW/glfw3.h>


namespace jdl
{
namespace core
{

class JDL_API Window : private NonCopyable<Window>
{
public:
    /**
     * @brief Creates the window.
     * @param title Window title
     * @param width Initial width
     * @param height Initial height
     */
    Window(const char* name, int width, int height);

    /**
     * @brief Destroys the window.
     */
    ~Window();

    /**
     * @brief Returns the window size, in screen coordinates.
     */
    Size getSize() const;

    /**
     * @brief Returns the window framebuffer size, in pixels.
     */
    Size getFramebufferSize() const;

    /**
     * @brief Returns whether the window closing flag has been set or not.
     */
    bool isClosing() const;

    /**
     * @brief Processes the events in the event queue and returns.
     */
    void pollEvents() const {
        glfwPollEvents();
    }

    /**
     * @brief Waits for events to be added in the event queue and processes them.
     */
    void waitEvents() const {
        glfwWaitEvents();
    }

    /**
     * @brief Returns the Vulkan instance extensions that are required to work with the GLFW window.
     */
    static std::vector<const char*> GetRequiredInstanceExtensions();

private:
    // Unique Window instance
    static Window* IWindow;

    // GLFW window pointer
    GLFWwindow* m_window = nullptr;
};

} // namespace core
} // namespace jdl
