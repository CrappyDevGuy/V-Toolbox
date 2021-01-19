//_Author : CrappyDevGuy, M01D19Y2021_//

#include "core/graphics/VtSampler.hpp"

#include "core/io/VtLogHandler.hpp"

VtSampler& VtSampler::operator=(VtSampler&& other) noexcept
{
	std::swap(m_name, other.m_name);
	std::swap(m_pVtDevices, other.m_pVtDevices);
	std::swap(m_sampler, other.m_sampler);
	std::swap(m_data, other.m_data);
	return *this;
}

VtSampler::VtSampler(VtSampler&& other) noexcept
:m_name{std::move(other.m_name)}
,m_pVtDevices{std::exchange(other.m_pVtDevices, nullptr)}
,m_sampler{std::exchange(other.m_sampler, nullptr)}
,m_data{std::move(other.m_data)}
{}

VtSampler::VtSampler(VtSamplerCreateInfo _createInfo)
{
	assert(_createInfo.pVtDevices != nullptr);
	
  m_name 			 = std::move(_createInfo.name);
  m_pVtDevices = _createInfo.pVtDevices;

	VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::~VtSampler", "Success to create");
}

void VtSampler::build()
{
	VkSamplerCreateInfo samplerInfo = {};
	samplerInfo.sType        					  = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter    					  = m_data.textureFilter;
	samplerInfo.minFilter    					  = m_data.textureFilter;
	samplerInfo.addressModeU 					  = m_data.textureWrap;
	samplerInfo.addressModeV 					  = m_data.textureWrap;
	samplerInfo.addressModeW 					  = m_data.textureWrap;
	samplerInfo.anisotropyEnable 				= (m_data.anisotropyLevel>0) ? VK_TRUE : VK_FALSE;
	samplerInfo.maxAnisotropy    				= m_data.anisotropyLevel;
	samplerInfo.borderColor  						= m_data.borderColor;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable 					= VK_FALSE;
	samplerInfo.compareOp     					= VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode 							= VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias 							= 0.0f;
	samplerInfo.minLod     							= 0.0f;
	samplerInfo.maxLod     							= m_data.mipLevels;
	
	VkResult res = vkCreateSampler(m_pVtDevices->getLogicalDevice(), &samplerInfo, nullptr, &m_sampler);
	VtUtil::checkVulkanResult(m_name+"::VtSampler::build", res);

	VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::~VtSampler", "Success to build");
}

VtSampler::~VtSampler()
{
	if(m_sampler != nullptr)
	{
		vkDestroySampler(m_pVtDevices->getLogicalDevice(), m_sampler, nullptr);
		m_sampler = nullptr;
		VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::~VtSampler", "Success to destroy VkSampler");
	}
}
