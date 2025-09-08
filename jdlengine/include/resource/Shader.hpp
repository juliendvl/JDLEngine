#pragma once

#include "Resource.hpp"


namespace jdl
{
namespace resource
{

enum class ShaderStage
{
    eVertex   = VK_SHADER_STAGE_VERTEX_BIT,
    eFragment = VK_SHADER_STAGE_FRAGMENT_BIT
};

class JDL_API Shader : public Resource
{
public:
    /**
     * @brief Creates the shader.
     * @param name Shader name
     * @param path Shader path
     */
    Shader(const std::string& name, const std::string& path);

    /**
     * @brief Returns the Vulkan shader module.
     */
    VkShaderModule getModule() const {
        return m_module;
    }

    /**
     * @brief Reloads the shader.
     */
    void reload();

private:
    std::string m_path;
    VkShaderModule m_module = VK_NULL_HANDLE;

    void createShader();
    void clearResourceData();
};

} // namespace resource
} // namespace jdl
