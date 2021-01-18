//_Author : CrappyDevGuy, M01D18Y2021_//

#include "core/graphics/VtCommandBuffer.hpp"
#include "core/io/VtLogHandler.hpp"
#include "core/util/VtUtil.hpp"

//_VtCommandBuffer_//
VtCommandBuffer& VtCommandBuffer::operator=(VtCommandBuffer&& other) noexcept
{
	std::swap(m_name, other.m_name);
	std::swap(m_pVtDevices, other.m_pVtDevices);
	std::swap(m_pVtCommandPool, other.m_pVtCommandPool);
	std::swap(m_commandBuffer, other.m_commandBuffer);
	return *this;
}

VtCommandBuffer::VtCommandBuffer(VtCommandBuffer&& other) noexcept
:m_name{std::move(other.m_name)}
,m_pVtDevices{std::exchange(other.m_pVtDevices, nullptr)}
,m_pVtCommandPool{std::exchange(other.m_pVtCommandPool, nullptr)}
,m_commandBuffer{std::exchange(other.m_commandBuffer, nullptr)}
{}

VtCommandBuffer::VtCommandBuffer(VtCommandBufferCreateInfo _createInfo)
{
	assert(_createInfo.pVtDevices != nullptr);

	m_name       = std::move(_createInfo.name);
	m_pVtDevices = _createInfo.pVtDevices;
  VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::VtCommandBuffer", "Success to create");
}

VtCommandBuffer::~VtCommandBuffer()
{
	if(m_commandBuffer != nullptr)
	{
		free();
	  VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::~VtCommandBuffer", "Success to destroy VkCommandBuffer");
	}
}

void VtCommandBuffer::alloc(VtCommandPool* pVtCommandPool)
{
	bindCommandPool(pVtCommandPool);
	
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool        = m_pVtCommandPool->getInstance();
	allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = 1;
		
	VtUtil::checkVulkanResult(m_name+"::VtCommandBuffer::alloc", vkAllocateCommandBuffers(m_pVtDevices->getLogicalDevice(), &allocInfo, &m_commandBuffer));
	VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::VtCommandBuffer::alloc", "Success to allocate");
}

void VtCommandBuffer::free()
{
	assert(m_pVtCommandPool != nullptr);
	vkFreeCommandBuffers(m_pVtDevices->getLogicalDevice(), m_pVtCommandPool->getInstance(), 1, &m_commandBuffer);
	m_commandBuffer = nullptr;
}

void VtCommandBuffer::begin()
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	VtUtil::checkVulkanResult(m_name+"::VtCommandBuffer::begin", vkBeginCommandBuffer(m_commandBuffer, &beginInfo));
}

void VtCommandBuffer::end()
{
	VtUtil::checkVulkanResult(m_name+"::VtCommandBuffer::begin", vkEndCommandBuffer(m_commandBuffer));
}

void VtCommandBuffer::reset()
{
	vkResetCommandBuffer(m_commandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
}

void VtCommandBuffer::beginRenderPass(VkRenderPassBeginInfo info, VkSubpassContents contents)
{
	vkCmdBeginRenderPass(m_commandBuffer, &info, contents);
}

void VtCommandBuffer::endRenderPass()
{
	vkCmdEndRenderPass(m_commandBuffer);
}

void VtCommandBuffer::bindPipeline(VtPipeline* pVtPipeline)
{
	m_tmpPipelineBindPoint = pVtPipeline->getBindPoint();
	vkCmdBindPipeline(m_commandBuffer, m_tmpPipelineBindPoint, pVtPipeline->getInstance());
}

void VtCommandBuffer::bindVertexBuffers(std::vector<VkBuffer> buffers, std::vector<VkDeviceSize> offsets)
{
  std::uint32_t bindingCount = static_cast<std::uint32_t>(buffers.size());
  vkCmdBindVertexBuffers(m_commandBuffer, 0, bindingCount, buffers.data(), offsets.data());
}

void VtCommandBuffer::bindVertexBuffer(VkBuffer& rBuffer, VkDeviceSize offset)
{
  vkCmdBindVertexBuffers(m_commandBuffer, 0, 1, &rBuffer, &offset);
}

void VtCommandBuffer::bindIndexBuffer(VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType)
{
	vkCmdBindIndexBuffer(m_commandBuffer, buffer, offset, indexType);
}

void VtCommandBuffer::bindPipelineLayout(VkPipelineLayout* pPipelineLayout)
{
	m_tmpPipelineLayout = pPipelineLayout;	
}

void VtCommandBuffer::bindDescriptorSet(std::uint32_t firstSet, VkDescriptorSet& set)
{
	assert(m_tmpPipelineLayout != nullptr);
  vkCmdBindDescriptorSets(m_commandBuffer, m_tmpPipelineBindPoint, *m_tmpPipelineLayout, firstSet, 1, &set, 0, nullptr);
}

void VtCommandBuffer::bindDescriptorSets(std::uint32_t firstSet, std::vector<VkDescriptorSet> sets)
{
  std::uint32_t count = static_cast<std::uint32_t>(sets.size());
  vkCmdBindDescriptorSets(m_commandBuffer, m_tmpPipelineBindPoint, *m_tmpPipelineLayout, firstSet, count, sets.data(), 0, nullptr);
}

void VtCommandBuffer::drawIndexed(std::uint32_t indicesCount, std::uint32_t instanceCount)
{
	vkCmdDrawIndexed(m_commandBuffer, indicesCount, instanceCount, 0, 0, 0);
}

void VtCommandBuffer::nextSubpass(VkSubpassContents contents)
{
  vkCmdNextSubpass(m_commandBuffer, contents);
}

void VtCommandBuffer::copyBuffer(VkBuffer& srcBuffer, VkBuffer& dstBuffer, VkDeviceSize size)
{
  begin();
    VkBufferCopy copyRegion = {};
    copyRegion.size = size;
    vkCmdCopyBuffer(m_commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
  end();

  VkSubmitInfo submitInfo = {};
  submitInfo.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers    = &m_commandBuffer;

  VkQueue TransferQueue = m_pVtDevices->getQueueByFlags(VK_QUEUE_GRAPHICS_BIT, true);
	
  vkQueueSubmit(TransferQueue, 1, &submitInfo, VK_NULL_HANDLE);
  vkQueueWaitIdle(TransferQueue);
}

void VtCommandBuffer::copyBuffers(std::vector<VkBuffer> srcBuffers, std::vector<VkBuffer> dstBuffers, std::vector<VkDeviceSize> sizes)
{
  begin();
    VkBufferCopy copyRegion = {};
    for(unsigned int i = 0; i < sizes.size(); i++)
    {
      copyRegion.size = sizes[i];
      vkCmdCopyBuffer(m_commandBuffer, srcBuffers[i], dstBuffers[i], 1, &copyRegion);
    }
  end();

  VkSubmitInfo submitInfo = {};
  submitInfo.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers    = &m_commandBuffer;

  VkQueue TransferQueue = m_pVtDevices->getQueueByFlags(VK_QUEUE_TRANSFER_BIT, true);
  vkQueueSubmit(TransferQueue, 1, &submitInfo, VK_NULL_HANDLE);
  vkQueueWaitIdle(TransferQueue);
}

//_VtCommandBufferUtil_//
