//_Author : CrappyDevGuy, M01D18Y2021_//

#include "core/graphics/VtPipeline.hpp"

#include "core/io/VtLogHandler.hpp"

VtPipeline& VtPipeline::operator=(VtPipeline&& other) noexcept
{
	std::swap(m_name, other.m_name);
	std::swap(m_pVtDevices, other.m_pVtDevices);
	std::swap(m_pipeline, other.m_pipeline);
	std::swap(m_states, other.m_states);
	return *this;
}

VtPipeline::VtPipeline(VtPipeline&& other) noexcept
:m_name{std::move(other.m_name)}
,m_pVtDevices{std::exchange(other.m_pVtDevices, nullptr)}
,m_pipeline{std::exchange(other.m_pipeline, nullptr)}
,m_states{std::move(other.m_states)}
{}

VtPipeline::VtPipeline(VtPipelineCreateInfo _createInfo)
{
	assert(_createInfo.pVtDevices != nullptr);

	m_name       = std::move(_createInfo.name);
	m_pVtDevices = _createInfo.pVtDevices;

	populateStateStructure();
	VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::VtPipeline", "Success to create");
}

VtPipeline::~VtPipeline()
{
	if(m_pipeline != nullptr)
	{
		vkDestroyPipeline(m_pVtDevices->getLogicalDevice(), m_pipeline, nullptr);
    m_pipeline = nullptr;
    VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::~VtPipeline", "Success to destroy VkPipeline");
	}
}

void VtPipeline::build(VtShader* pVtShader, VtRenderpass* pVtRenderpass, VtPipelineLayout* pVtPipelineLayout)
{
  assert(pVtShader != nullptr);
  assert(pVtRenderpass != nullptr);
  assert(pVtPipelineLayout != nullptr);

 	VkPipelineViewportStateCreateInfo viewportCreate = {};
 	viewportCreate.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportCreate.viewportCount = 1;
	viewportCreate.pViewports    = &m_states.viewport;
	viewportCreate.scissorCount  = 1;
	viewportCreate.pScissors     = &m_states.scissor;

	VkPipelineShaderStageCreateInfo shaderStages[] = {pVtShader->getVertexShaderStage(), pVtShader->getFragmentShaderStage()};
	auto vertexInput = pVtShader->getPipelineVertexInputState();
	
	VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
	pipelineCreateInfo.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineCreateInfo.stageCount          = 2;
	pipelineCreateInfo.pStages             = shaderStages;
  pipelineCreateInfo.pVertexInputState   = &vertexInput;
	pipelineCreateInfo.pInputAssemblyState = &m_states.inAsmCreate;
	pipelineCreateInfo.pViewportState      = &viewportCreate;
	pipelineCreateInfo.pRasterizationState = &m_states.razCreate;
	pipelineCreateInfo.pMultisampleState   = &m_states.msCreate;
	pipelineCreateInfo.pDepthStencilState  = &m_states.depthStencil;
	pipelineCreateInfo.pColorBlendState    = &m_states.colorBlending;
	pipelineCreateInfo.layout              = pVtPipelineLayout->getInstance();
	
	pipelineCreateInfo.renderPass          = pVtRenderpass->getInstance();
	pipelineCreateInfo.subpass             = 0;
	pipelineCreateInfo.basePipelineHandle  = VK_NULL_HANDLE;
	pipelineCreateInfo.basePipelineIndex   = -1;
		
	VkResult res = vkCreateGraphicsPipelines(m_pVtDevices->getLogicalDevice(), nullptr, 1, &pipelineCreateInfo, nullptr, &m_pipeline);
	VtUtil::checkVulkanResult(m_name+"::VtPipeline::build", res);
	
	VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::VtPipeline::build", "Success to build");	
}

void VtPipeline::setViewport(glm::ivec2 viewport) noexcept
{
	m_states.viewport.width  = static_cast<std::uint32_t>(viewport.x);
	m_states.viewport.height = static_cast<std::uint32_t>(viewport.y);
}

void VtPipeline::setViewport(VkExtent2D viewport) noexcept
{
	m_states.viewport.width  = viewport.width;
	m_states.viewport.height = viewport.height;
}

void VtPipeline::setScissorExtent(glm::ivec2 extent) noexcept
{
	m_states.scissor.extent.width  = static_cast<std::uint32_t>(extent.x);
	m_states.scissor.extent.height = static_cast<std::uint32_t>(extent.y);
}

void VtPipeline::setScissorExtent(VkExtent2D extent) noexcept
{
	m_states.scissor.extent = extent;
}

//_Private Functions_//
void VtPipeline::populateStateStructure()
{
	m_states.scissor.offset = {0, 0};
	m_states.scissor.extent = {800, 600};
	
	m_states.viewport.x        = 0.0f;
	m_states.viewport.y        = 0.0f;
	m_states.viewport.width    = 800;
	m_states.viewport.height   = 600;
	m_states.viewport.minDepth = 0.0f;
	m_states.viewport.maxDepth = 1.0f;
	
	m_states.razCreate.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	m_states.razCreate.depthClampEnable        = VK_FALSE;
	m_states.razCreate.depthBiasEnable         = VK_FALSE;
	m_states.razCreate.rasterizerDiscardEnable = VK_FALSE;
	m_states.razCreate.polygonMode             = VK_POLYGON_MODE_FILL;
	m_states.razCreate.lineWidth               = 1.0f;
	m_states.razCreate.cullMode                = VK_CULL_MODE_FRONT_BIT;
	m_states.razCreate.frontFace               = VK_FRONT_FACE_CLOCKWISE;
		
	m_states.msCreate.sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	m_states.msCreate.sampleShadingEnable   = VK_FALSE;
	m_states.msCreate.rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT;
	m_states.msCreate.minSampleShading      = 1.0f;
	m_states.msCreate.pSampleMask           = nullptr;
	m_states.msCreate.alphaToCoverageEnable = VK_FALSE;
	m_states.msCreate.alphaToOneEnable      = VK_FALSE;

	m_states.depthStencil.sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	m_states.depthStencil.depthTestEnable       = VK_TRUE;
	m_states.depthStencil.depthWriteEnable      = VK_TRUE;
	m_states.depthStencil.depthCompareOp        = VK_COMPARE_OP_LESS;
	m_states.depthStencil.depthBoundsTestEnable = VK_FALSE;
	m_states.depthStencil.minDepthBounds        = 0.0f;
	m_states.depthStencil.maxDepthBounds        = 1.0f;
	m_states.depthStencil.stencilTestEnable     = VK_FALSE;
	m_states.depthStencil.front                 = {};
	m_states.depthStencil.back                  = {};

	m_states.colorBlendAttachment.blendEnable    = VK_FALSE;
  m_states.colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
		                                      		   VK_COLOR_COMPONENT_G_BIT |
		                                      		   VK_COLOR_COMPONENT_B_BIT |
		                                      		   VK_COLOR_COMPONENT_A_BIT;
		
	m_states.colorBlending.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	m_states.colorBlending.logicOpEnable     = VK_FALSE;
	m_states.colorBlending.logicOp           = VK_LOGIC_OP_COPY;
	m_states.colorBlending.attachmentCount   = 1;
	m_states.colorBlending.pAttachments      = &m_states.colorBlendAttachment;
	m_states.colorBlending.blendConstants[0] = 0.0f;
	m_states.colorBlending.blendConstants[1] = 0.0f;
	m_states.colorBlending.blendConstants[2] = 0.0f;
	m_states.colorBlending.blendConstants[3] = 0.0f;

	m_states.inAsmCreate.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	m_states.inAsmCreate.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	m_states.inAsmCreate.primitiveRestartEnable = VK_FALSE;
}
