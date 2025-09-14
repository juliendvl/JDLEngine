#pragma once

#include "Core.hpp"

#include "utils/NonCopyable.hpp"


namespace jdl
{
namespace core
{

class JDL_API RenderPass : private NonCopyable<RenderPass>
{
public:
    /**
     * @brief Creates the render pass.
     */
    RenderPass();

    /**
     * @brief Destroys the render pass.
     */
    ~RenderPass();

    /**
     * @brief Returns the render pass Vulkan handle.
     */
    VkRenderPass getHandle() const {
        return m_renderPass;
    }

private:
    VkRenderPass m_renderPass = VK_NULL_HANDLE;
};

} // namespace core
} // namespace jdl
