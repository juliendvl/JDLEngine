#pragma once

#include "Core.hpp"

#include "utils/NonCopyable.hpp"


namespace jdl
{
namespace core
{

struct ApplicationArgs
{
    std::string name;   /*!< Application name */
    size_t width = 0;   /*!< Window initial width */
    size_t height = 0;  /*!< Window initial height */
};

class JDL_API Application : private NonCopyable<Application>
{
public:
    /**
     * @brief Creates the application.
     * 
     * @param name Application name.
     * @param width Window initial width.
     * @param height Window initial height.
     */
    Application(const std::string& name, size_t width, size_t height);

    /**
     * @brief Returns the application instance.
     */
    static Application& Get() {
        return *IApplication;
    }

    /**
     * @brief Returns the application name.
     */
    static const std::string& GetName() {
        return IApplication->m_name;
    }

    /**
     * @brief Runs the application.
     */
    void run();

private:
    // Single Application instance
    static Application* IApplication;
    // Application name
    std::string m_name;
};

/**
 * @brief Creates and returns the unique application instance.
 * This function must be implemented by any application using the engine.
 * 
 * @param args Application arguments.
 * @return The created application.
 */
std::unique_ptr<Application> CreateApplication(const ApplicationArgs& args);

} // namespace core
} // namespace jdl
