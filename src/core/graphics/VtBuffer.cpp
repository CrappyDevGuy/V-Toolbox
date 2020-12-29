//_Author : CrappyDevGuy, M12D29Y2020_//

#include "core/graphics/VtBuffer.hpp"

#include "core/io/VtLogHandler.hpp"

VtBuffer& VtBuffer::operator=(VtBuffer&& other) noexcept
{
	std::swap(other.m_name, m_name);
  std::swap(other.m_pVtDevices, m_pVtDevices);
  std::swap(other.m_buffer, m_buffer);
  std::swap(other.m_memory, m_memory);
  std::swap(other.m_view, m_view);
  std::swap(other.m_size, m_size);
  return *this;
}

VtBuffer::VtBuffer(VtBuffer&& other) noexcept
:m_buffer{std::exchange(other.m_buffer, nullptr)}
,m_memory{std::exchange(other.m_memory, nullptr)}
,m_view{std::exchange(other.m_view, nullptr)}
,m_pVtDevices{std::exchange(other.m_pVtDevices, nullptr)}
,m_name{std::move(other.m_name)}
,m_size{std::move(other.m_size)}
{}

VtBuffer::VtBuffer(VtBufferCreateInfo _createInfo)
{
	assert(_createInfo.pVtDevices != nullptr);

	m_name 		   = std::move(_createInfo.name);
	m_size 			 = std::move(_createInfo.size);
	m_pVtDevices = _createInfo.pVtDevices;

	VkBufferCreateInfo bufferInfo = {};
  bufferInfo.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size        = m_size;
  bufferInfo.usage       = _createInfo.usage;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  VtUtil::checkVulkanResult(m_name+"::VtBuffer::Create", vkCreateBuffer(m_pVtDevices->getLogicalDevice(), &bufferInfo, nullptr, &m_buffer));

  VkMemoryRequirements memRequirements;
  vkGetBufferMemoryRequirements(m_pVtDevices->getLogicalDevice(), m_buffer, &memRequirements);

  VkMemoryAllocateInfo allocInfo = {};
  allocInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize  = memRequirements.size;
  allocInfo.memoryTypeIndex = m_pVtDevices->findMemoryType(memRequirements.memoryTypeBits, _createInfo.properties, m_name);

  VtUtil::checkVulkanResult(m_name+"::VtBuffer::Allocation", vkAllocateMemory(m_pVtDevices->getLogicalDevice(), &allocInfo, nullptr, &m_memory));
  VtUtil::checkVulkanResult(m_name+"::VtBuffer::Binding", vkBindBufferMemory(m_pVtDevices->getLogicalDevice(), m_buffer, m_memory, 0));

	VtLogHandler::oStream("V-Toolbox", m_name+"::VtBuffer", "Success to create");	
}

VtBuffer::~VtBuffer()
{
	if(m_buffer != nullptr)
  {
    vkDestroyBuffer(m_pVtDevices->getLogicalDevice(), m_buffer, nullptr);
    m_buffer = nullptr;
    VtLogHandler::oStreamDebug("V-Toolbox", m_name+"~VtBuffer", "Success to destroy VkBuffer");
  }

  if(m_memory != nullptr)
  {
    vkFreeMemory(m_pVtDevices->getLogicalDevice(), m_memory, nullptr);
    m_memory = nullptr;
    VtLogHandler::oStreamDebug("V-Toolbox", m_name+"~VtBuffer", "Success to destroy VkDeviceMemory");
  }

  if(m_view != nullptr)
  {
    vkDestroyBufferView(m_pVtDevices->getLogicalDevice(), m_view, nullptr);
    m_view = nullptr;
    VtLogHandler::oStreamDebug("V-Toolbox", m_name+"~VtBuffer", "Success to destroy VkBufferView");
  }
}

void VtBuffer::createView(VkFormat format, VkDeviceSize offset, VkDeviceSize range)
{
  VkBufferViewCreateInfo view_createInfo = {};
  view_createInfo.sType  = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
  view_createInfo.buffer = m_buffer;
  view_createInfo.format = format;
  view_createInfo.offset = offset;
  view_createInfo.range  = range;
  VtUtil::checkVulkanResult(m_name+"::VtBuffer::createView", vkCreateBufferView(m_pVtDevices->getLogicalDevice(), &view_createInfo, nullptr, &m_view));
	VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::VtBuffer::createView", "Success to create");
}

void VtBuffer::copyToImage(VkCommandBuffer& commandBuffer, VtImage& rImage)
{
	VkBufferImageCopy region = {};
  region.bufferOffset      = 0;
  region.bufferRowLength   = 0;
  region.bufferImageHeight = 0;

  region.imageSubresource.aspectMask     = rImage.getAspect();
  region.imageSubresource.mipLevel       = 0;
  region.imageSubresource.baseArrayLayer = 0;
  region.imageSubresource.layerCount     = 1;

  glm::ivec2 extent = rImage.getExtent();

  std::uint32_t w = static_cast<std::uint32_t>(extent.x);
  std::uint32_t h = static_cast<std::uint32_t>(extent.y);

  region.imageOffset = {0,0,0};
  region.imageExtent = {w, h, 1};

  vkCmdCopyBufferToImage(
    commandBuffer,
    m_buffer,
    rImage.getImage(),
    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
    1,
    &region
  );
}
