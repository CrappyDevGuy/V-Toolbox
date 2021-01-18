//_Author : CrappyDevGuy, M01D18Y2021_//

#include "core/graphics/VtPipeline.hpp"

#include "core/io/VtLogHandler.hpp"

VtPipeline& VtPipeline::operator=(VtPipeline&& other) noexcept
{
	std::swap(m_name, other.m_name);
	std::swap(m_pVtDevices, other.m_pVtDevices);
	std::swap(m_pipeline, other.m_pipeline);
	return *this;
}

VtPipeline::VtPipeline(VtPipeline&& other) noexcept
:m_name{std::move(other.m_name)}
,m_pVtDevices{std::exchange(other.m_pVtDevices, nullptr)}
,m_pipeline{std::exchange(other.m_pipeline, nullptr)}
{}

VtPipeline::VtPipeline(VtPipelineCreateInfo _createInfo)
{
	assert(_createInfo.pVtDevices != nullptr);

	m_name       = std::move(_createInfo.name);
	m_pVtDevices = _createInfo.pVtDevices;

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

  VkPipelineShaderStageCreateInfo shaderStages[] = {pVtShader->getVertexShaderStage(), pVtShader->getFragmentShaderStage()};
  
  VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
  vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  if(!pVtShader->getShaderInput().isEmpty())
  {
    vertexInputInfo.vertexBindingDescriptionCount   = pVtShader->getShaderInput().getBindingsSize();
    vertexInputInfo.vertexAttributeDescriptionCount = pVtShader->getShaderInput().getAttributesSize();
    vertexInputInfo.pVertexBindingDescriptions      = pVtShader->getShaderInput().getBindingInfo().data();
    vertexInputInfo.pVertexAttributeDescriptions    = pVtShader->getShaderInput().getAttributesInfo().data();
  }else
  {
    vertexInputInfo.vertexBindingDescriptionCount   = 0;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions      = nullptr;
    vertexInputInfo.pVertexAttributeDescriptions    = nullptr;
  }

	VkPipelineInputAssemblyStateCreateInfo inAsmCreateInfo = {};
	  inAsmCreateInfo.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	  inAsmCreateInfo.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	  inAsmCreateInfo.primitiveRestartEnable = VK_FALSE;

		VkExtent2D extent;
		extent.width  = static_cast<std::uint32_t>(m_parameters.viewport.x);
		extent.height = static_cast<std::uint32_t>(m_parameters.viewport.y);
	
	  VkViewport viewport = {};
	  viewport.x        = 0.0f;
	  viewport.y        = 0.0f;
	  viewport.width    = extent.width;
	  viewport.height   = extent.height;
	  viewport.minDepth = 0.0f;
	  viewport.maxDepth = 1.0f;
	
	  VkRect2D scissor = {};
	  scissor.offset   = {0,0};
	  scissor.extent   = extent;
	
	  VkPipelineViewportStateCreateInfo viewportState = {};
	  viewportState.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	  viewportState.viewportCount = 1;
	  viewportState.pViewports    = &viewport;
	  viewportState.scissorCount  = 1;
	  viewportState.pScissors     = &scissor;
	
	  VkPipelineRasterizationStateCreateInfo razCreateInfo = {};
	  razCreateInfo.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	  razCreateInfo.depthClampEnable        = VK_FALSE;
	  razCreateInfo.depthBiasEnable         = VK_FALSE;
	  razCreateInfo.rasterizerDiscardEnable = VK_FALSE;
	  razCreateInfo.polygonMode             = m_parameters.polygonMode;
	  razCreateInfo.lineWidth               = m_parameters.lineWidth;
	  razCreateInfo.cullMode                = m_parameters.cullFace;
	  razCreateInfo.frontFace               = VK_FRONT_FACE_CLOCKWISE;
	
	  VkPipelineMultisampleStateCreateInfo msCreateInfo = {};
	  msCreateInfo.sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	  msCreateInfo.sampleShadingEnable   = VK_FALSE;
	  msCreateInfo.rasterizationSamples  = m_parameters.msaaValue;
	  msCreateInfo.minSampleShading      = 1.0f;
	  msCreateInfo.pSampleMask           = nullptr;
	  msCreateInfo.alphaToCoverageEnable = VK_FALSE;
	  msCreateInfo.alphaToOneEnable      = VK_FALSE;
	
	  VkPipelineDepthStencilStateCreateInfo depthStencilInfo = {};
	  depthStencilInfo.sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	  depthStencilInfo.depthTestEnable       = VK_TRUE;
	  depthStencilInfo.depthWriteEnable      = VK_TRUE;
	  depthStencilInfo.depthCompareOp        = VK_COMPARE_OP_LESS;
	  depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
	  depthStencilInfo.minDepthBounds        = 0.0f;
	  depthStencilInfo.maxDepthBounds        = 1.0f;
	  depthStencilInfo.stencilTestEnable     = VK_FALSE;
	  depthStencilInfo.front                 = {};
	  depthStencilInfo.back                  = {};
	
	  VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	  colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
	                                        VK_COLOR_COMPONENT_G_BIT |
	                                        VK_COLOR_COMPONENT_B_BIT |
	                                        VK_COLOR_COMPONENT_A_BIT;
	  colorBlendAttachment.blendEnable    = m_parameters.blend;
	
	  VkPipelineColorBlendStateCreateInfo colorBlending = {};
	  colorBlending.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	  colorBlending.logicOpEnable     = VK_FALSE;
	  colorBlending.logicOp           = VK_LOGIC_OP_COPY;
	  colorBlending.attachmentCount   = 1;
	  colorBlending.pAttachments      = &colorBlendAttachment;
	  colorBlending.blendConstants[0] = 0.0f;
	  colorBlending.blendConstants[1] = 0.0f;
	  colorBlending.blendConstants[2] = 0.0f;
	  colorBlending.blendConstants[3] = 0.0f;
	
	  VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
	  pipelineCreateInfo.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	
	  //_Shader_//
	  pipelineCreateInfo.stageCount          = 2;
	  pipelineCreateInfo.pStages             = shaderStages;
	
	  //_Input Vertex_//
	  pipelineCreateInfo.pVertexInputState   = &vertexInputInfo;
	
	  //_Input Assembly_//
	  pipelineCreateInfo.pInputAssemblyState = &inAsmCreateInfo;
	
	  //_Viewport_//
	  pipelineCreateInfo.pViewportState      = &viewportState;
	
	  //_Rasterization_//
	  pipelineCreateInfo.pRasterizationState = &razCreateInfo;
	
	  //_MultiSample_//
	  pipelineCreateInfo.pMultisampleState   = &msCreateInfo;
	
	  //_Depth_//
	  pipelineCreateInfo.pDepthStencilState  = &depthStencilInfo;
	
	  //_Color Blending_//
	  pipelineCreateInfo.pColorBlendState    = &colorBlending;
	
	  //_Layout_//
	  pipelineCreateInfo.layout              = pVtPipelineLayout->getInstance();
	
	  pipelineCreateInfo.renderPass          = pVtRenderpass->getInstance();
	  pipelineCreateInfo.subpass             = 0;
	  pipelineCreateInfo.basePipelineHandle  = VK_NULL_HANDLE;
	  pipelineCreateInfo.basePipelineIndex   = -1;
	
	  VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
	  pipelineCacheCreateInfo.sType           = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
	  pipelineCacheCreateInfo.initialDataSize = 0;
	  pipelineCacheCreateInfo.pInitialData    = nullptr;
	
	  VkResult res = vkCreateGraphicsPipelines(m_pVtDevices->getLogicalDevice(), nullptr, 1, &pipelineCreateInfo, nullptr, &m_pipeline);
		VtUtil::checkVulkanResult(m_name+"::VtPipeline::build", res);
	
		VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::VtPipeline::build", "Success to build");	
}
