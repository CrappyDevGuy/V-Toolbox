//_Author : CrappyDevGuy, M01D18Y2021_//

#include "core/graphics/VtFramebuffer.hpp"

#include "core/io/VtLogHandler.hpp"
#include "core/util/VtUtil.hpp"

VtFramebuffer& VtFramebuffer::operator=(VtFramebuffer&& other) noexcept
{
	std::swap(m_name, other.m_name);
	std::swap(m_pVtDevices, other.m_pVtDevices);
	std::swap(m_framebuffer, other.m_framebuffer);
	std::swap(m_extent, other.m_extent);
	return *this;
}

VtFramebuffer::VtFramebuffer(VtFramebuffer&& other) noexcept
:m_name{std::move(other.m_name)}
,m_pVtDevices{std::exchange(other.m_pVtDevices, nullptr)}
,m_framebuffer{std::exchange(other.m_framebuffer, nullptr)}
,m_extent{std::move(other.m_extent)}
{}

VtFramebuffer::VtFramebuffer(VtFramebufferCreateInfo _createInfo)
{
	assert(_createInfo.pVtDevices != nullptr);
  assert(_createInfo.pVtRenderpass != nullptr);
  assert(_createInfo.imagesView.size() > 0);

	m_name       = std::move(_createInfo.name);
	m_pVtDevices = _createInfo.pVtDevices;
	m_extent     = std::move(_createInfo.extent);

  VkFramebufferCreateInfo framebufferInfo = {};
  framebufferInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
  framebufferInfo.renderPass      = _createInfo.pVtRenderpass->getInstance();
  framebufferInfo.attachmentCount = static_cast<std::uint32_t>(_createInfo.imagesView.size());
  framebufferInfo.pAttachments    = _createInfo.imagesView.data();
  framebufferInfo.width           = static_cast<std::uint32_t>(m_extent.x);
  framebufferInfo.height          = static_cast<std::uint32_t>(m_extent.y);
  framebufferInfo.layers          = 1;

  VkResult res = vkCreateFramebuffer(m_pVtDevices->getLogicalDevice(), &framebufferInfo, nullptr, &m_framebuffer);
	VtUtil::checkVulkanResult(m_name+"::VtFramebuffer", res);
  
	VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::VtFramebuffer", "Success to create");
}

VtFramebuffer::~VtFramebuffer()
{
	if(m_framebuffer != nullptr)
	{
		vkDestroyFramebuffer(m_pVtDevices->getLogicalDevice(), m_framebuffer, nullptr);
		m_framebuffer = nullptr;
		VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::~VtFramebuffer", "Success to destroy VkFramebuffer");
	}
}
