#include <core/Application.hpp>
#include <core/EntryPoint.hpp>

using namespace jdl;


class Sandbox : public core::Application
{
public:
    Sandbox(const std::string& name, size_t width, size_t height)
        : core::Application(name, width, height)
    {
        std::cout << "Hello World!" << std::endl;
    }
};

std::unique_ptr<core::Application> core::CreateApplication(const core::ApplicationArgs& args)
{
    return std::make_unique<Sandbox>(args.name, args.width, args.height);
}
