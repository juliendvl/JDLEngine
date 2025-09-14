#include "core/Pipeline.hpp"
#include "core/VulkanContext.hpp"

#include "resource/ResourceManager.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace core
{

Pipeline::~Pipeline()
{
    if (m_pipeline != VK_NULL_HANDLE)
    {
        VkDevice device = VulkanContext::GetDevice();

        vkDestroyPipelineLayout(device, m_pipelineLayout, nullptr);
        vkDestroyPipeline(device, m_pipeline, nullptr);
    }
    m_pipeline = VK_NULL_HANDLE;
}

void Pipeline::addShader(resource::Shader* shader, ShaderStage stage)
{
    if (m_pipeline != VK_NULL_HANDLE)
    {
        JDL_ERROR("Cannot add shader on an existing pipeline");
        return;
    }
    m_shaders[stage] = shader;
}

void Pipeline::addShader(const std::string& shader, ShaderStage stage)
{
    if (m_pipeline != VK_NULL_HANDLE)
    {
        JDL_ERROR("Cannot add shader on an existing pipeline");
        return;
    }

    resource::Shader* resource = resource::ResourceManager::GetAs<resource::Shader>(shader);
    if (resource == nullptr)
    {
        JDL_ERROR("Shader {} does not exist", shader);
        return;
    }

    addShader(resource, stage);
}

void Pipeline::setPrimitiveType(PrimitiveType type)
{
    if (m_pipeline != VK_NULL_HANDLE)
    {
        JDL_ERROR("Cannot set primitive type on an existing pipeline");
        return;
    }
    m_primitiveType = type;
}

void Pipeline::create()
{
    if (m_pipeline != VK_NULL_HANDLE)
    {
        JDL_ERROR("Pipeline has already been created");
        return;
    }

    createPipelineLayout();

    // Shaders
    std::vector<VkPipelineShaderStageCreateInfo> shadersInfo;
    for (const auto& [stage, shader] : m_shaders)
    {
        VkPipelineShaderStageCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        createInfo.stage = static_cast<VkShaderStageFlagBits>(stage);
        createInfo.module = shader->getModule();
        createInfo.pName = "main";

        shadersInfo.push_back(createInfo);
    }

    // Dynamic state
    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicState = {};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    // Vertex Input
    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = nullptr;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr;

    // Input assembly
    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = static_cast<VkPrimitiveTopology>(m_primitiveType);
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    // Viewport/Scissors
    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    // Rasterization
    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = static_cast<VkPolygonMode>(m_polygonMode);
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_NONE;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    // Multisampling
    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    // Color blending
    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT
        | VK_COLOR_COMPONENT_G_BIT
        | VK_COLOR_COMPONENT_B_BIT
        | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = static_cast<uint32_t>(shadersInfo.size());
    pipelineInfo.pStages = shadersInfo.data();
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = nullptr;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = m_pipelineLayout;
    pipelineInfo.renderPass = VulkanContext::GetRenderPass().getHandle();
    pipelineInfo.subpass = 0;

    VkDevice device = VulkanContext::GetDevice();
    VK_CALL(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline));
}

void Pipeline::createPipelineLayout()
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

    VkDevice device = VulkanContext::GetDevice();
    VK_CALL(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &m_pipelineLayout));
}

} // namespace core
} // namespace jdl
