#pragma once

#include "core/Core.hpp"

#include "utils/NonCopyable.hpp"


namespace jdl
{
namespace vk
{

class JDL_API VulkanRenderer : private NonCopyable<VulkanRenderer>
{
public:
    /**
     * @brief Inits the renderer.
     */
    VulkanRenderer();

    /**
     * @brief Destroys the renderer.
     */
    ~VulkanRenderer();

    /**
     * @brief Renders a new frame.
     */
    void renderFrame();
};

} // namespace vk
} // namespace jdl
