//_Author : CrappyDevGuy, M01D18Y2021_//

#include "core/graphics/layout/VtDescriptorLayout.hpp"
#include "core/io/VtLogHandler.hpp"
#include "core/util/VtUtil.hpp"

VtDescriptorLayout& VtDescriptorLayout::operator=(VtDescriptorLayout&& other) noexcept
{
	std::swap(m_name, other.m_name);
	std::swap(m_pVtDevices, other.m_pVtDevices);
	std::swap(m_descriptorlayout, other.m_descriptorlayout);
	std::swap(m_bindings, other.m_bindings);
	return *this;
}

VtDescriptorLayout::VtDescriptorLayout(VtDescriptorLayout&& other) noexcept
:m_name{std::move(other.m_name)}
,m_pVtDevices{std::exchange(other.m_pVtDevices, nullptr)}
,m_descriptorlayout{std::exchange(other.m_descriptorlayout, nullptr)}
,m_bindings{std::move(other.m_bindings)}
{}

VtDescriptorLayout::VtDescriptorLayout(VtDescriptorLayoutCreateInfo _createInfo)
{
	assert(_createInfo.pVtDevices != nullptr);

	m_name 			 = std::move(_createInfo.name);
	m_pVtDevices = _createInfo.pVtDevices;

	VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::VtDescriptorLayout", "Success to create");
}

VtDescriptorLayout::~VtDescriptorLayout()
{
	if(m_descriptorlayout != nullptr)
	{
		vkDestroyDescriptorSetLayout(m_pVtDevices->getLogicalDevice(), m_descriptorlayout, nullptr);
		m_descriptorlayout = nullptr;
		VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::~VtDescriptorLayout", "Success to destroy VkDescriptorSetLayout");
	}
}

void VtDescriptorLayout::build()
{
  VkDescriptorSetLayoutCreateInfo layoutInfo = {};
  layoutInfo.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  layoutInfo.bindingCount = static_cast<std::uint32_t>(m_bindings.size());
  layoutInfo.pBindings    = m_bindings.data();

  VkResult res = vkCreateDescriptorSetLayout(m_pVtDevices->getLogicalDevice(), &layoutInfo, nullptr, &m_descriptorlayout);
	VtUtil::checkVulkanResult(m_name+"::VtDescriptorLayout::build", res);

	VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::VtDescriptorLayout::build", "Success to build");
}

void VtDescriptorLayout::addBinding(VtDescriptorLayoutBindingInfo info)
{
  VkDescriptorSetLayoutBinding binding = {};

  binding.binding         = info.binding;
  binding.descriptorType  = info.dataType;
  binding.descriptorCount = info.dataCount;
  binding.stageFlags      = info.shaderStage;
  m_bindings.push_back(binding);
}

void VtDescriptorLayout::addBindings(std::vector<VtDescriptorLayoutBindingInfo> info)
{
  std::uint32_t size = m_bindings.size();
  m_bindings.reserve(size+info.size());

  for(auto c_info : info)
    addBinding(c_info);
}
