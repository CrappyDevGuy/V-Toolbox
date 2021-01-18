//_Author : CrappyDevGuy, M01D18Y2021_//

#include "core/graphics/VtShader.hpp"

#include "core/io/VtLogHandler.hpp"
#include "core/util/VtUtil.hpp"

VtShader& VtShader::operator=(VtShader&& other) noexcept
{
	std::swap(m_name, other.m_name);
	std::swap(m_pVtDevices, other.m_pVtDevices);
	std::swap(m_vertShader, other.m_vertShader);
	std::swap(m_fragShader, other.m_fragShader);
	std::swap(m_geomShader, other.m_geomShader);
	std::swap(m_shaderInput, other.m_shaderInput);
	return *this;
}

VtShader::VtShader(VtShader&& other) noexcept
:m_name{std::move(other.m_name)}
,m_pVtDevices{std::exchange(other.m_pVtDevices, nullptr)}
,m_vertShader{std::exchange(other.m_vertShader, nullptr)}
,m_fragShader{std::exchange(other.m_fragShader, nullptr)}
,m_geomShader{std::exchange(other.m_geomShader, nullptr)}
,m_shaderInput{std::move(other.m_shaderInput)}
{}

VtShader::VtShader(VtShaderCreateInfo _createInfo)
{
	assert(_createInfo.pVtDevices != nullptr);

	assert(_createInfo.vertShader != "NULL");

	m_name        = std::move(_createInfo.name);
	m_pVtDevices  = _createInfo.pVtDevices;
	m_shaderInput = std::move(_createInfo.shaderInput);
	
  std::vector<char> vertData;
  std::vector<char> fragData;

  VtUtil::file::load(_createInfo.vertShader, vertData);
  VtUtil::file::load(_createInfo.fragShader, fragData);

  createShaderModule(m_vertShader, "VERT", vertData);
  createShaderModule(m_fragShader, "FRAG", fragData);

  if(_createInfo.geomShader != "NULL")
  {
  	std::vector<char> geomData;
		VtUtil::file::load(_createInfo.geomShader, geomData);
		createShaderModule(m_geomShader, "GEOM", geomData);
  }

  VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::VtShader", "Success to create");
}

VtShader::~VtShader()
{
	destroyShaderModule(&m_vertShader, "VERT");
	destroyShaderModule(&m_fragShader, "FRAG");
	destroyShaderModule(&m_geomShader, "GEOM");
}

VkPipelineShaderStageCreateInfo VtShader::getVertexShaderStage()
{
  VkPipelineShaderStageCreateInfo createInfo = {};
  createInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  createInfo.stage  = VK_SHADER_STAGE_VERTEX_BIT;
  createInfo.module = m_vertShader;
  createInfo.pName  = "main";
  return std::move(createInfo);
}

VkPipelineShaderStageCreateInfo VtShader::getFragmentShaderStage()
{
  VkPipelineShaderStageCreateInfo createInfo = {};
  createInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  createInfo.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
  createInfo.module = m_fragShader;
  createInfo.pName  = "main";
  return std::move(createInfo);
}

VkPipelineShaderStageCreateInfo VtShader::getGeometryShaderStage()
{
  VkPipelineShaderStageCreateInfo createInfo = {};
  createInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  createInfo.stage  = VK_SHADER_STAGE_GEOMETRY_BIT;
  createInfo.module = m_geomShader;
  createInfo.pName  = "main";
  return std::move(createInfo);
}

//_ Private Functions _//

void VtShader::createShaderModule(VkShaderModule& module, const std::string type, const std::vector<char>& code)
{
  VkShaderModuleCreateInfo createInfo = {};
  createInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode    = reinterpret_cast<const std::uint32_t*>(code.data());

  VtUtil::checkVulkanResult(m_name+"::createShaderModule::"+type, vkCreateShaderModule(m_pVtDevices->getLogicalDevice(), &createInfo, nullptr, &module));
}

void VtShader::destroyShaderModule(VkShaderModule* module, const std::string type)
{
  if(module != nullptr)
  {
    vkDestroyShaderModule(m_pVtDevices->getLogicalDevice(), *module, nullptr);
    module = nullptr;
    VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::destroyShaderModule::"+type, "Success to destroy VkShaderModule");
  }
}
