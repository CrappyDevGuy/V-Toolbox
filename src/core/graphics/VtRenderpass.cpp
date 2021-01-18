//_Author : CrappyDevGuy, M01D18Y2021_//

#include "core/graphics/VtRenderpass.hpp"
#include "core/io/VtLogHandler.hpp"
#include "core/util/VtUtil.hpp"

VtRenderpass& VtRenderpass::operator=(VtRenderpass&& other) noexcept
{
	std::swap(m_name, other.m_name);
	std::swap(m_pVtDevices, other.m_pVtDevices);
	std::swap(m_renderpass, other.m_renderpass);
	return *this;
}

VtRenderpass::VtRenderpass(VtRenderpass&& other) noexcept
:m_name{std::move(other.m_name)}
,m_pVtDevices{std::exchange(other.m_pVtDevices, nullptr)}
,m_renderpass{std::exchange(other.m_renderpass, nullptr)}
{}

VtRenderpass::VtRenderpass(VtRenderpassCreateInfo _createInfo)
{
	assert(_createInfo.pVtDevices != nullptr);
	assert(_createInfo.pVtSubpass != nullptr);

	m_name 			 = std::move(_createInfo.name);
	m_pVtDevices = _createInfo.pVtDevices;

	auto descriptions = _createInfo.pVtSubpass->getDescriptions();
	auto dependencies = _createInfo.pVtSubpass->getDependencies();
	
	std::vector<VkAttachmentDescription> attachments(_createInfo.attachments.size());
	for(auto i = 0u; i < attachments.size(); i++)
	{
	  attachments[i].format         = _createInfo.attachments[i].format;
	  attachments[i].samples        = _createInfo.attachments[i].samples;
	  attachments[i].loadOp         = _createInfo.attachments[i].loadOp;
	  attachments[i].storeOp        = _createInfo.attachments[i].storeOp;
	  attachments[i].stencilLoadOp  = _createInfo.attachments[i].stencilLoadOp;
	  attachments[i].stencilStoreOp = _createInfo.attachments[i].stencilStoreOp;
	  attachments[i].initialLayout  = _createInfo.attachments[i].initialLayout;
	  attachments[i].finalLayout    = _createInfo.attachments[i].finalLayout;
	}
	
	VkRenderPassCreateInfo renderpassInfo = {};
	renderpassInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderpassInfo.attachmentCount = static_cast<std::uint32_t>(attachments.size());
	renderpassInfo.pAttachments    = attachments.data();
	renderpassInfo.subpassCount    = static_cast<std::uint32_t>(descriptions.size());
	renderpassInfo.pSubpasses      = descriptions.data();
	
	if(dependencies.size()>0)
	{
	  renderpassInfo.dependencyCount = static_cast<std::uint32_t>(dependencies.size());
	  renderpassInfo.pDependencies   = dependencies.data();
	}
	
	VkResult res = vkCreateRenderPass(m_pVtDevices->getLogicalDevice(), &renderpassInfo, nullptr, &m_renderpass);
	VtUtil::checkVulkanResult(m_name+"::VtRenderpass", res);
	
	VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::VtRenderpass", "Success to create");
}

VtRenderpass::~VtRenderpass()
{
	if(m_renderpass != nullptr)
  {
    vkDestroyRenderPass(m_pVtDevices->getLogicalDevice(), m_renderpass, nullptr);
    m_renderpass = nullptr;
    VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::~VtRenderpass", "Success to destroy VkRenderPass");
 }
}
