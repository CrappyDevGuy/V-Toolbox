//_Author : CrappyDevGuy, M01D03Y2021_//

#include "core/graphics/pool/VtDescriptorPool.hpp"

#include "core/io/VtLogHandler.hpp"

VtDescriptorPool& VtDescriptorPool::operator=(VtDescriptorPool&& other) noexcept
{
	std::swap(m_name, other.m_name);
	std::swap(m_setsCount, other.m_setsCount);
	std::swap(m_pVtDevices, other.m_pVtDevices);
	std::swap(m_descriptorpool, other.m_descriptorpool);
	return *this;	
}

VtDescriptorPool::VtDescriptorPool(VtDescriptorPool&& other) noexcept
:m_name{std::move(other.m_name)}
,m_setsCount{other.m_setsCount}
,m_pVtDevices{std::exchange(other.m_pVtDevices, nullptr)}
,m_descriptorpool{std::exchange(other.m_descriptorpool, nullptr)}
{}

VtDescriptorPool::VtDescriptorPool(VtDescriptorPoolCreateInfo _createInfo)
{
	assert(_createInfo.pVtDevices != nullptr);

	m_name       = std::move(_createInfo.name);
	m_setsCount  = _createInfo.setsCount;
	m_pVtDevices = _createInfo.pVtDevices;

  VkDescriptorPoolCreateInfo poolInfo = {};
  poolInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  poolInfo.poolSizeCount = static_cast<std::uint32_t>(_createInfo.poolsData.size());
  poolInfo.pPoolSizes    = _createInfo.poolsData.data();
  poolInfo.maxSets       = m_setsCount;

  VtUtil::checkVulkanResult(m_name+"::VtDescriptorPool::vkCreateDescriptorPool", vkCreateDescriptorPool(m_pVtDevices->getLogicalDevice(), &poolInfo, nullptr, &m_descriptorpool));
	
	VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::VtDescriptorPool", "Success to create");

  #ifdef VT_DEBUG_DEFINE
		for(auto i = 0; i < _createInfo.poolsData.size(); i++)
		{
			std::string poolToString = m_name+"::pool[" + std::to_string(i)+"] : "
									  					 + VtUtil::VkDescriptorTypeFlagToString(_createInfo.poolsData[i].type);

			VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::VtDescriptorPool", poolToString);
		}
  #endif
}

VtDescriptorPool::~VtDescriptorPool()
{
	if(m_descriptorpool != nullptr)
	{
	  vkDestroyDescriptorPool(m_pVtDevices->getLogicalDevice(), m_descriptorpool, nullptr);	
		m_descriptorpool = nullptr;
		VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::~VtDescriptorPool", "Success to destroy VkDescriptorPool");
	}
}
