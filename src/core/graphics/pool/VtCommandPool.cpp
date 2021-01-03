//_Author : CrappyDevGuy, M01D03Y2021_//

#include "core/graphics/pool/VtCommandPool.hpp"

#include "core/io/VtLogHandler.hpp"

VtCommandPool& VtCommandPool::operator=(VtCommandPool&& other) noexcept
{
	std::swap(m_name, other.m_name);
	std::swap(m_pVtDevices, other.m_pVtDevices);
	std::swap(m_commandpool, other.m_commandpool);
	return *this;
}


VtCommandPool::VtCommandPool(VtCommandPool&& other) noexcept
:m_name{std::move(other.m_name)}
,m_pVtDevices{std::exchange(other.m_pVtDevices, nullptr)}
,m_commandpool{std::exchange(other.m_commandpool, nullptr)}
{}

VtCommandPool::VtCommandPool(VtCommandPoolCreateInfo _createInfo)
{
	assert(_createInfo.pVtDevices != nullptr);

	m_name       = std::move(_createInfo.name);
	m_pVtDevices = _createInfo.pVtDevices;

  VkCommandPoolCreateInfo poolInfo = {};
  poolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.queueFamilyIndex = _createInfo.queueFamilyIndex;
  poolInfo.flags            = _createInfo.flags;

  VtUtil::checkVulkanResult(m_name+"::VtCommandPool::vkCreateCommandPool", vkCreateCommandPool(m_pVtDevices->getLogicalDevice(), &poolInfo, nullptr, &m_commandpool));

	VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::VtCommandPool", "Success to create, flags : " + VtUtil::VkCommandPoolFlagsToString(_createInfo.flags));
}

VtCommandPool::~VtCommandPool()
{
	if(m_commandpool != nullptr)
	{
    vkDestroyCommandPool(m_pVtDevices->getLogicalDevice(), m_commandpool, nullptr);
		m_commandpool = nullptr;
		VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::~VtCommandPool", "Success to destroy VkCommandPool");
	}
}
