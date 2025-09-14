#pragma once

#include "Core.hpp"

#include "resource/Shader.hpp"

#include "utils/NonCopyable.hpp"

#include <unordered_map>


namespace jdl
{
namespace core
{

enum class PolygonMode
{
    ePoint = VK_POLYGON_MODE_POINT,
    eLine  = VK_POLYGON_MODE_LINE,
    eFill  = VK_POLYGON_MODE_FILL
};

enum class PrimitiveType
{
    ePoints        = VK_PRIMITIVE_TOPOLOGY_POINT_LIST,
    eLines         = VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
    eLineStrip     = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP,
    eTriangles     = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
    eTriangleStrip = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP
};

enum class ShaderStage
{
    eVertex   = VK_SHADER_STAGE_VERTEX_BIT,
    eFragment = VK_SHADER_STAGE_FRAGMENT_BIT
};

class JDL_API Pipeline : private NonCopyable<Pipeline>
{
public:
    /**
     * @brief Inits the graphics pipeline, but does not create it.
     */
    Pipeline() = default;

    /**
     * @brief Destroys the graphics pipeline.
     */
    ~Pipeline();

    /**
     * @brief Adds a new shader to the pipeline.
     * @param shader The shader to be added/
     * @param stage Shader stage
     */
    void addShader(resource::Shader* shader, ShaderStage stage);
    void addShader(const std::string& shader, ShaderStage stage);

    /**
     * @brief Returns the pipeline primitive type.
     */
    PrimitiveType getPrimitiveType() const {
        return m_primitiveType;
    }

    /**
     * @brief Sets the pipeline primitive type.
     * @param type The new type
     */
    void setPrimitiveType(PrimitiveType type);

    /**
     * @brief Returns the pipeline polygon mode.
     */
    PolygonMode getPolygonMode() const {
        return m_polygonMode;
    }

    /**
     * @brief Sets the pipeline polygon mode.
     * @param mode The new mode
     */
    void setPolygonMode(PolygonMode mode);

    /**
     * @brief Creates the graphics pipeline.
     */
    void create();

    /**
     * @brief Returns the pipeline layout handle.
     */
    VkPipelineLayout getPipelineLayout() const {
        return m_pipelineLayout;
    }

    /**
     * @brief Returns the pipeline Vulkan handle.
     */
    VkPipeline getHandle() const {
        return m_pipeline;
    }

private:
    // Shaders
    std::unordered_map<ShaderStage, resource::Shader*> m_shaders;
    // Pipeline data
    PrimitiveType m_primitiveType = PrimitiveType::eTriangles;
    PolygonMode m_polygonMode = PolygonMode::eFill;

    // Vulkan objects
    VkPipeline m_pipeline = VK_NULL_HANDLE;
    VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;

    void createPipelineLayout();
    void createRenderPass();
};

} // namespace core
} // namespace jdl
