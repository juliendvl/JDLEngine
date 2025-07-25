#include <iostream>

#include <core/Application.hpp>
#include <core/EntryPoint.hpp>


class Sandbox : public jdl::core::Application
{
public:
    Sandbox(const char* name, int width, int height)
        : jdl::core::Application(name, width, height)
    {
        std::cout << "Hello World!" << std::endl;
    }
};

std::unique_ptr<jdl::core::Application> CreateApplication(const char* name, int width, int height)
{
    return std::make_unique<Sandbox>(name, width, height);
}
