//_Author : CrappyDevGuy, M01D18Y2021_//

#include "core/graphics/layout/VtPipelineLayout.hpp"
#include "core/io/VtLogHandler.hpp"
#include "core/util/VtUtil.hpp"

VtPipelineLayout& VtPipelineLayout::operator=(VtPipelineLayout&& other) noexcept
{
	std::swap(m_name, other.m_name);
  std::swap(m_pVtDevices, other.m_pVtDevices);
  std::swap(m_pipelineLayout, other.m_pipelineLayout);
  std::swap(m_layouts, other.m_layouts);
  return *this; 
}

VtPipelineLayout::VtPipelineLayout(VtPipelineLayout&& other) noexcept
:m_name{std::move(other.m_name)}
,m_pVtDevices{std::exchange(other.m_pVtDevices, nullptr)}
,m_pipelineLayout{std::move(other.m_pipelineLayout)}
,m_layouts{std::move(other.m_layouts)}
{}

VtPipelineLayout::VtPipelineLayout(VtPipelineLayoutCreateInfo _createInfo)
{
	assert(_createInfo.pVtDevices != nullptr);

	m_name 		   = std::move(_createInfo.name);
	m_pVtDevices = _createInfo.pVtDevices;

	VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::VtPipelineLayout", "Success to create");	
}

VtPipelineLayout::~VtPipelineLayout()
{
	if(m_pipelineLayout != nullptr)
	{
	  vkDestroyPipelineLayout(m_pVtDevices->getLogicalDevice(), m_pipelineLayout, nullptr);
		m_pipelineLayout = nullptr;
		VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::VtPipelineLayout", "Success to destroy VkPipelineLayout");
	}
}

void VtPipelineLayout::build()
{
	assert(!m_layouts.empty());

	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
  pipelineLayoutCreateInfo.sType          = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutCreateInfo.pSetLayouts    = m_layouts.data();
  pipelineLayoutCreateInfo.setLayoutCount = static_cast<std::uint32_t>(m_layouts.size());

  VkResult res = vkCreatePipelineLayout(m_pVtDevices->getLogicalDevice(), &pipelineLayoutCreateInfo, nullptr, &m_pipelineLayout);
	VtUtil::checkVulkanResult(m_name+"::VtPipelineLayout::build", res);

	VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::VtPipelineLayout::build", "Success to build");
}

void VtPipelineLayout::addDescriptorLayout(VtDescriptorLayout* pDescriptorLayout)
{
	assert(pDescriptorLayout != nullptr);
	m_layouts.push_back(pDescriptorLayout->getInstance());
}
