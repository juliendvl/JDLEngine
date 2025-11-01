#include <core/Application.hpp>
#include <core/EntryPoint.hpp>

#include <utils/Logger.hpp>

using namespace jdl;


class Sandbox : public core::Application
{
public:
    Sandbox(const std::string& name, size_t width, size_t height)
        : core::Application(name, width, height)
    {
        JDL_TRACE("TRACE Message");
        JDL_DEBUG("DEBUG Message");
        JDL_INFO("INFO Message");
        JDL_WARN("WARN Message");
        JDL_ERROR("ERROR Message");
    }
};

std::unique_ptr<core::Application> core::CreateApplication(const core::ApplicationArgs& args)
{
    return std::make_unique<Sandbox>(args.name, args.width, args.height);
}
