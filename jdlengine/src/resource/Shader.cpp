#include "resource/Shader.hpp"

#include "core/VulkanContext.hpp"

#include "utils/Logger.hpp"

#include <fstream>


namespace jdl
{
namespace resource
{

Shader::Shader(const std::string& name, const std::string& path)
    : Resource(name)
    , m_path(path)
{
    createShader();
}

void Shader::createShader()
{
    // Read the shader code
    std::ifstream stream(m_path, std::ios::binary | std::ios::ate);
    if (!stream.is_open()) {
        JDL_ERROR("Cannot open shader file {}", m_path);
        return;
    }

    size_t fileSize = static_cast<size_t>(stream.tellg());
    stream.seekg(0);

    std::vector<char> code(fileSize);
    stream.read(code.data(), fileSize);

    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = fileSize;
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkDevice device = core::VulkanContext::GetDevice();
    VK_CALL(vkCreateShaderModule(device, &createInfo, nullptr, &m_module));
}

void Shader::reload()
{
    clearResourceData();
    createShader();
}

void Shader::clearResourceData()
{
    VkDevice device = core::VulkanContext::GetDevice();
    vkDestroyShaderModule(device, m_module, nullptr);

    m_module = VK_NULL_HANDLE;
}

} // namespace resource
} // namespace jdl
