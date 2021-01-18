//_Author : CrappyDevGuy, M01D18Y2021_//

#include "core/graphics/VtDescriptorSet.hpp"

#include "core/io/VtLogHandler.hpp"
#include "core/util/VtUtil.hpp"

VtDescriptorSet& VtDescriptorSet::operator=(VtDescriptorSet&& other) noexcept
{
	std::swap(m_name, other.m_name);
	std::swap(m_pVtDevices, other.m_pVtDevices);
	std::swap(m_descriptor, other.m_descriptor);

	return *this;
}

VtDescriptorSet::VtDescriptorSet(VtDescriptorSet&& other) noexcept
:m_name{std::move(other.m_name)}
,m_pVtDevices{std::exchange(other.m_pVtDevices, nullptr)}
,m_descriptor{std::exchange(other.m_descriptor, nullptr)}
{}

VtDescriptorSet::VtDescriptorSet(VtDescriptorSetCreateInfo _createInfo)
{
	assert(_createInfo.pVtDevices != nullptr);

	m_name 			 = std::move(_createInfo.name);
	m_pVtDevices = _createInfo.pVtDevices;
}

VtDescriptorSet::~VtDescriptorSet()
{
	if(m_descriptor != nullptr)
	{
		m_descriptor = nullptr;
		VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::~VtDescriptorSet", "Success to destroy VkDescriptorSet");
	}
}

void VtDescriptorSet::build(VtDescriptorPool* pVtDescriptorPool, VtDescriptorLayout* pVtDescriptorLayout)
{
  assert(pVtDescriptorPool != nullptr);
	assert(pVtDescriptorLayout != nullptr);

  VkDescriptorSetAllocateInfo allocInfo = {};
  allocInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocInfo.descriptorPool     = pVtDescriptorPool->getInstance();
  allocInfo.descriptorSetCount = 1;
  allocInfo.pSetLayouts        = &pVtDescriptorLayout->getInstance();

  VkResult res = vkAllocateDescriptorSets(m_pVtDevices->getLogicalDevice(), &allocInfo, &m_descriptor);
	VtUtil::checkVulkanResult(m_name+"::VtDescriptorSet::build", res);

	VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::VtDescriptorSet::build", "Success to create");
}

void VtDescriptorSet::update()
{
  assert(m_descriptor != nullptr);

  assert(!m_descriptorWrites.empty());  
  
  std::vector<VkWriteDescriptorSet> writesData;

  for(auto& c_binding : m_descriptorWrites)
  {
    VkWriteDescriptorSet descriptorWrite = {};
    descriptorWrite.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstBinding      = c_binding.binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType  = c_binding.descriptorType;

    if(!c_binding.buffersInfo.empty()) 
    { 
      descriptorWrite.descriptorCount = static_cast<std::uint32_t>(c_binding.buffersInfo.size()); 
      descriptorWrite.pBufferInfo     = c_binding.buffersInfo.data();
    } 
    else if(!c_binding.imagesInfo.empty()) 
    { 
      descriptorWrite.descriptorCount = static_cast<std::uint32_t>(c_binding.imagesInfo.size());       
      descriptorWrite.pImageInfo      = c_binding.imagesInfo.data();
    }

    descriptorWrite.dstSet = m_descriptor;
    writesData.push_back(descriptorWrite);
  }
  std::uint32_t size = static_cast<std::uint32_t>(writesData.size());

  vkUpdateDescriptorSets(m_pVtDevices->getLogicalDevice(), size, writesData.data(), 0, nullptr);
}

void VtDescriptorSet::addBinding(VtDescriptorSetBinding t_binding, VkDescriptorImageInfo imageInfo)
{
  VtDescriptorSet::BindingData bindingData = {};
  bindingData.binding         = t_binding.binding;
  bindingData.descriptorType  = t_binding.descriptorType;
  bindingData.imagesInfo.push_back(std::move(imageInfo));

  m_descriptorWrites.push_back(std::move(bindingData));
}

void VtDescriptorSet::addBinding(VtDescriptorSetBinding t_binding, std::vector<VkDescriptorImageInfo> imagesInfo)
{
  VtDescriptorSet::BindingData bindingData = {};
  bindingData.binding         = t_binding.binding;
  bindingData.descriptorType  = t_binding.descriptorType;
  bindingData.imagesInfo      = imagesInfo;

  m_descriptorWrites.push_back(std::move(bindingData));
}

void VtDescriptorSet::addBinding(VtDescriptorSetBinding t_binding, VkDescriptorBufferInfo bufferInfo)
{
  VtDescriptorSet::BindingData bindingData = {};
  bindingData.binding         = t_binding.binding;
  bindingData.descriptorType  = t_binding.descriptorType;
  bindingData.buffersInfo.push_back(std::move(bufferInfo));

  m_descriptorWrites.push_back(std::move(bindingData));
}

void VtDescriptorSet::addBinding(VtDescriptorSetBinding t_binding, std::vector<VkDescriptorBufferInfo> buffersInfo)
{
  VtDescriptorSet::BindingData bindingData = {};
  bindingData.binding         = t_binding.binding;
  bindingData.descriptorType  = t_binding.descriptorType;
  bindingData.buffersInfo     = buffersInfo;

  m_descriptorWrites.push_back(std::move(bindingData));
}
