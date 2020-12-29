//_Author : CrappyDevGuy, M12D29Y2020_//

#include "core/graphics/VtImage.hpp"

#include "core/io/VtLogHandler.hpp"
#include "core/util/VtUtil.hpp"
#include "core/graphics/VtBuffer.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

VtImage& VtImage::operator=(VtImage&& other) noexcept
{
	std::swap(m_pVtDevices, other.m_pVtDevices);
	std::swap(m_image, other.m_image);
	std::swap(m_imageView, other.m_imageView);
	std::swap(m_imageMemory, other.m_imageMemory);
	std::swap(m_data, other.m_data);
	return *this;
}

VtImage::VtImage(VtImage&& other) noexcept
:m_pVtDevices{std::exchange(other.m_pVtDevices, nullptr)}
,m_image{std::exchange(other.m_image, nullptr)}
,m_imageView{std::exchange(other.m_imageView, nullptr)}
,m_imageMemory{std::exchange(other.m_imageMemory, nullptr)}
,m_data{std::move(other.m_data)}
{}

VtImage::VtImage(VtImageCreateInfo _createInfo)
{
	assert(_createInfo.pVtDevices != nullptr);

	m_pVtDevices  = _createInfo.pVtDevices;
	m_data.name   = std::move(_createInfo.name);
	m_data.format = std::move(_createInfo.format);

	VtLogHandler::oStream("V-Toolbox", m_data.name+"::VtImage", "Success to create");
}

VtImage::~VtImage()
{
	if(!m_data.hasBeenSet && m_image != nullptr)
	{
		vkDestroyImage(m_pVtDevices->getLogicalDevice(), m_image, nullptr);
		m_image = nullptr;
		VtLogHandler::oStream("V-Toolbox", m_data.name+"::~VtImage", "Success to destroy VkImage");
	}

	if(m_imageView != nullptr)
	{
	  vkDestroyImageView(m_pVtDevices->getLogicalDevice(), m_imageView, nullptr);		
		m_imageView = nullptr;
		VtLogHandler::oStreamDebug("V-Toolbox", m_data.name+"::~VtImage", "Success to destroy VkImageView");
	}

	if(m_imageMemory != nullptr)
	{
    vkFreeMemory(m_pVtDevices->getLogicalDevice(), m_imageMemory, nullptr);
  	m_imageMemory = nullptr;
		VtLogHandler::oStreamDebug("V-Toolbox", m_data.name+"::~VtImage", "Success to destroy VkImageMemory");
	}
	
}

void VtImage::createImage(VtImageInfo _imageInfo)
{
  assert(m_pVtDevices != nullptr);

	m_data.extent       = std::move(_imageInfo.extent);
  m_data.samplesCount = std::move(_imageInfo.samplesCount);
  m_data.hasMipMaps   = _imageInfo.createMipMaps;

  if(m_data.hasMipMaps)
    m_data.mipLevels = std::floor(std::log2(std::max(m_data.extent.x, m_data.extent.y)))+1;

  VkImageCreateInfo imageInfo = {};
  imageInfo.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  imageInfo.imageType     = VK_IMAGE_TYPE_2D;
  imageInfo.extent.width  = static_cast<std::uint32_t>(m_data.extent.x);
  imageInfo.extent.height = static_cast<std::uint32_t>(m_data.extent.y);
  imageInfo.extent.depth  = 1;
  imageInfo.mipLevels     = m_data.mipLevels;
  imageInfo.arrayLayers   = 1;
  imageInfo.format        = m_data.format;
  imageInfo.tiling        = _imageInfo.tiling;
  imageInfo.initialLayout = _imageInfo.initialLayout;
  imageInfo.usage         = _imageInfo.usage;
  imageInfo.samples       = m_data.samplesCount;
  imageInfo.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;

  VtUtil::checkVulkanResult(m_data.name+"::VtImage::createImage", vkCreateImage(m_pVtDevices->getLogicalDevice(), &imageInfo, nullptr, &m_image));

  m_data.hasBeenSet = false;
	VtLogHandler::oStreamDebug("V-Toolbox", m_data.name+"::VtImage::createImage", "Success to create");
}

void VtImage::createView(VkImageAspectFlags aspect)
{
  assert(m_pVtDevices != nullptr);
  assert(m_image != nullptr);

  m_data.aspect = std::move(aspect);
  
  VkImageViewCreateInfo viewInfo = {};
  viewInfo.sType    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
  viewInfo.image    = m_image;
  viewInfo.format   = m_data.format;

  viewInfo.subresourceRange.aspectMask     = m_data.aspect;
  viewInfo.subresourceRange.baseMipLevel   = 0;
  viewInfo.subresourceRange.levelCount     = m_data.mipLevels;
  viewInfo.subresourceRange.baseArrayLayer = 0;
  viewInfo.subresourceRange.layerCount     = 1;

  VtUtil::checkVulkanResult(m_data.name+"::VtImage::createView", vkCreateImageView(m_pVtDevices->getLogicalDevice(), &viewInfo, nullptr, &m_imageView));  
	VtLogHandler::oStreamDebug("V-Toolbox", m_data.name+"::VtImage::createView", "Success to create");
}

void VtImage::createMemory(VkMemoryPropertyFlags t_properties)
{
  assert(m_pVtDevices != nullptr);
  assert(m_image != nullptr);

  VkMemoryRequirements memRequirements;
  vkGetImageMemoryRequirements(m_pVtDevices->getLogicalDevice(), m_image, &memRequirements);

  std::uint32_t memoryIndex = 0;
  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(m_pVtDevices->getPhysicalDevice(), &memProperties);

  for(std::uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    if(memRequirements.memoryTypeBits & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & t_properties) == t_properties)
    {
      memoryIndex = i;
      break;
    }

  VkMemoryAllocateInfo allocInfo = {};
  allocInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize  = memRequirements.size;
  allocInfo.memoryTypeIndex = memoryIndex;

  VtUtil::checkVulkanResult(m_data.name+"::VtImage::createMemory::Allocation", vkAllocateMemory(m_pVtDevices->getLogicalDevice(), &allocInfo, nullptr, &m_imageMemory));
  VtUtil::checkVulkanResult(m_data.name+"::VtImage::createMemory::Binding", vkBindImageMemory(m_pVtDevices->getLogicalDevice(), m_image, m_imageMemory, 0));
	VtLogHandler::oStreamDebug("V-Toolbox", m_data.name+"::VtImage::createMemory", "Success to create");
}

void VtImage::transitionLayout(VkCommandBuffer& commandBuffer, VkImageLayout t_oldLayout, VkImageLayout t_newLayout)
{
  assert(m_image != nullptr);

  VkImageMemoryBarrier barrier = {};
  barrier.sType     = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barrier.oldLayout = t_oldLayout;
  barrier.newLayout = t_newLayout;
  barrier.image     = m_image;

  barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

  barrier.subresourceRange.aspectMask     = m_data.aspect;
  barrier.subresourceRange.baseMipLevel   = 0;
  barrier.subresourceRange.levelCount     = m_data.mipLevels;
  barrier.subresourceRange.baseArrayLayer = 0;
  barrier.subresourceRange.layerCount     = 1;

  VkPipelineStageFlags srcStage, dstStage;

  bool isOldUndefined = t_oldLayout == VK_IMAGE_LAYOUT_UNDEFINED;
  bool isNewTransfer  = t_newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

  bool isOldTransfer  = t_oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  bool isNewShader    = t_newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

  if(isOldUndefined && isNewTransfer)
  {
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

    srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
  }else if(isOldTransfer && isNewShader)
  {
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  }else
		VtLogHandler::oStreamFatalError("V-Toolbox", m_data.name+"::VtImage::transitionLayout", "unsupported Transition Layout");

	VtLogHandler::oStreamDebug("V-Toolbox", m_data.name+"::VtImage::transitionLayout", "Transition Layout");

  vkCmdPipelineBarrier(
    commandBuffer,
    srcStage, dstStage,
    0,
    0, nullptr,
    0, nullptr,
    1, &barrier
  );
}

void VtImage::loadImage(VkCommandBuffer& commandBuffer, std::string path)
{
  if(!VtUtil::file::exist(path))
    VtLogHandler::oStreamFatalError("V-Toolbox", m_data.name+"::VtImage::loadImage", "couldn't found, path = " + path);

  int textureWidth, textureHeight;
  stbi_uc* pixels = stbi_load(path.c_str(), &textureWidth, &textureHeight, nullptr, STBI_rgb_alpha);
  if(!pixels)
    VtLogHandler::oStreamFatalError("V-Toolbox", m_data.name+"::VtImage::loadImage", "failed to load, path = " + path);

  VkDeviceSize imageSize = textureWidth*textureHeight*4;

  VtBuffer stagingBuffer{{m_data.name+"_StagingBuffer", m_pVtDevices, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT}};
  stagingBuffer.mapMemory(pixels);

  m_data.format       = VK_FORMAT_R8G8B8A8_SRGB;
  m_data.aspect       = VK_IMAGE_ASPECT_COLOR_BIT;
  m_data.channelCount = 4;

 	VtImageInfo info = {};
  info.extent        = glm::ivec2{textureWidth, textureHeight};
  info.createMipMaps = true;
  info.tiling        = VK_IMAGE_TILING_OPTIMAL;
  info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  info.usage         = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

  VkCommandBufferBeginInfo beginInfo = {};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
  
  createImage(std::move(info));
  createMemory(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  vkBeginCommandBuffer(commandBuffer, &beginInfo);
    transitionLayout(commandBuffer, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    createView(VK_IMAGE_ASPECT_COLOR_BIT);

    stagingBuffer.copyToImage(commandBuffer, *this);
    generateMipMaps(commandBuffer);
  vkEndCommandBuffer(commandBuffer);

  VkSubmitInfo submitInfo = {};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;

  vkQueueSubmit(m_pVtDevices->getQueue(0), 1, &submitInfo, VK_NULL_HANDLE);
  vkQueueWaitIdle(m_pVtDevices->getQueue(0));

  stbi_image_free(pixels);
  VtLogHandler::oStream("V-Toolbox", m_data.name+"::VtImage::loadImage", "Success to load, path = " + path);

}

void VtImage::generateMipMaps(VkCommandBuffer& commandBuffer)
{
  assert(m_image != nullptr);

  VkFormatProperties formatProperties;
  vkGetPhysicalDeviceFormatProperties(m_pVtDevices->getPhysicalDevice(), m_data.format, &formatProperties);

  if(!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
  	VtLogHandler::oStreamFatalError("V-Toolbox", m_data.name+"::VtImage::generateMipMaps", "the texture format do not support the linear filter");

  VkImageMemoryBarrier barrier = {};
  barrier.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barrier.image                           = m_image;
  barrier.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
  barrier.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
  barrier.subresourceRange.aspectMask     = m_data.aspect;
  barrier.subresourceRange.baseArrayLayer = 0;
  barrier.subresourceRange.layerCount     = 1;
  barrier.subresourceRange.levelCount     = 1;

  std::int32_t mipWidth  = static_cast<std::uint32_t>(m_data.extent.x);
  std::int32_t mipHeight = static_cast<std::uint32_t>(m_data.extent.y);

  for(std::uint32_t i = 1; i < m_data.mipLevels; i++)
  {
    barrier.subresourceRange.baseMipLevel = i-1;
    barrier.oldLayout     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout     = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

    vkCmdPipelineBarrier(commandBuffer,
      VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
      0, nullptr,
      0, nullptr,
      1, &barrier
    );

    VkImageBlit blit = {};
    blit.srcOffsets[0]                 = {0,0,0};
    blit.srcOffsets[1]                 = {mipWidth,mipHeight,1};
    blit.srcSubresource.aspectMask     = m_data.aspect;
    blit.srcSubresource.mipLevel       = i-1;
    blit.srcSubresource.baseArrayLayer = 0;
    blit.srcSubresource.layerCount     = 1;

    std::int32_t offsetX = mipWidth > 1 ? mipWidth/2 : 1;
    std::int32_t offsetY = mipHeight > 1 ? mipHeight/2 : 1;

    blit.dstOffsets[0]                 = {0,0,0};
    blit.dstOffsets[1]                 = {offsetX,offsetY,1};
    blit.dstSubresource.aspectMask     = m_data.aspect;
    blit.dstSubresource.mipLevel       = i;
    blit.dstSubresource.baseArrayLayer = 0;
    blit.dstSubresource.layerCount     = 1;

    vkCmdBlitImage(commandBuffer,
      m_image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
      m_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      1, &blit,
      VK_FILTER_LINEAR
    );

    barrier.oldLayout     = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    barrier.newLayout     = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    vkCmdPipelineBarrier(commandBuffer,
      VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
      0, nullptr,
      0, nullptr,
      1, &barrier
    );

    if(mipWidth > 1) mipWidth /= 2;
    if(mipHeight > 1) mipHeight /= 2;
  }
  barrier.subresourceRange.baseMipLevel = m_data.mipLevels-1;
  barrier.oldLayout     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  barrier.newLayout     = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
  barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

  vkCmdPipelineBarrier(commandBuffer,
    VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
    0, nullptr,
    0, nullptr,
    1, &barrier
  );
	
  VtLogHandler::oStreamDebug("V-Toolbox", m_data.name+"::VtImage::generateMipMaps", "Success to create " + std::to_string(m_data.mipLevels) + " level of mipmaps");
}
