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

	VtLogHandler::oStream("V-Toolbox", m_name+"::VtPipeline", "Success to create");
}

VtPipeline::~VtPipeline()
{
	if(m_pipeline != nullptr)
	{
		vkDestroyPipeline(m_pVtDevices->getLogicalDevice(), m_pipeline, nullptr);
    m_pipeline = nullptr;
    VtLogHandler::oStream("V-Toolbox", m_name+"::~VtPipeline", "Success to destroy VkPipeline");
	}
}

void VtPipeline::build(VtShader* pShader)
{
  assert(pShader != nullptr);

  VkPipelineShaderStageCreateInfo shaderStages[] = {pShader->getVertexShaderStage(), pShader->getFragmentShaderStage()};
  
  VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
  vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  if(!pShader->getShaderInput().isEmpty())
  {
    vertexInputInfo.vertexBindingDescriptionCount   = pShader->getShaderInput().getBindingsSize();
    vertexInputInfo.vertexAttributeDescriptionCount = pShader->getShaderInput().getAttributesSize();
    vertexInputInfo.pVertexBindingDescriptions      = pShader->getShaderInput().getBindingInfo().data();
    vertexInputInfo.pVertexAttributeDescriptions    = pShader->getShaderInput().getAttributesInfo().data();
  }else
  {
    vertexInputInfo.vertexBindingDescriptionCount   = 0;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions      = nullptr;
    vertexInputInfo.pVertexAttributeDescriptions    = nullptr;
  }
	
}
