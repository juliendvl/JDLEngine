#include <iostream>

#include <core/Application.hpp>
#include <core/EntryPoint.hpp>

#include <resource/ResourceManager.hpp>
#include <resource/Shader.hpp>

using namespace jdl;


class Sandbox : public core::Application
{
public:
    Sandbox(const char* name, int width, int height)
        : core::Application(name, width, height)
    {
        resource::ResourceManager::Create<resource::Shader>("default_vert", "shaders/default.vert.spv");
        resource::ResourceManager::Create<resource::Shader>("default_frag", "shaders/default.frag.spv");
    }
};

std::unique_ptr<jdl::core::Application> CreateApplication(const char* name, int width, int height)
{
    return std::make_unique<Sandbox>(name, width, height);
}
