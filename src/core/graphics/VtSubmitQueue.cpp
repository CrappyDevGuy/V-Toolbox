//_Author : CrappyDevGuy, M01D18Y2021_//

#include "core/graphics/VtSubmitQueue.hpp"

#include "core/io/VtLogHandler.hpp"
#include "core/util/VtUtil.hpp"

VtSubmitQueue& VtSubmitQueue::operator=(VtSubmitQueue&& other) noexcept
{
	std::swap(m_name, other.m_name);
	std::swap(m_pVtDevices, other.m_pVtDevices);
	std::swap(m_pVtSwapchain, other.m_pVtSwapchain);
  std::swap(m_imagesAvailable, other.m_imagesAvailable);
  std::swap(m_rendersFinished, other.m_rendersFinished);
  std::swap(m_inflightFences, other.m_inflightFences);
  std::swap(m_imagesinflight, other.m_imagesinflight);
  std::swap(m_currentFrame, other.m_currentFrame);
  std::swap(m_imageCount, other.m_imageCount);
	return *this;
}

VtSubmitQueue::VtSubmitQueue(VtSubmitQueue&& other) noexcept
:m_name{std::move(other.m_name)}
,m_pVtDevices{std::exchange(other.m_pVtDevices, nullptr)}
,m_pVtSwapchain{std::exchange(other.m_pVtSwapchain, nullptr)}
,m_imagesAvailable{std::move(other.m_imagesAvailable)}
,m_rendersFinished{std::move(other.m_rendersFinished)}
,m_inflightFences{std::move(other.m_inflightFences)}
,m_imagesinflight{std::move(other.m_imagesinflight)}
,m_currentFrame{std::move(other.m_currentFrame)}
,m_imageCount{std::move(other.m_imageCount)}
{}

VtSubmitQueue::VtSubmitQueue(VtSubmitQueueCreateInfo _createInfo)
{
	assert(_createInfo.pVtDevices != nullptr);
	assert(_createInfo.pVtSwapchain != nullptr);

	m_name 			   = std::move(_createInfo.name);
	m_pVtDevices   = _createInfo.pVtDevices;
	m_pVtSwapchain = _createInfo.pVtSwapchain;

	m_imageCount = m_pVtSwapchain->getImageCount();
	
	m_imagesAvailable.resize(m_imageCount);
	m_rendersFinished.resize(m_imageCount);
	m_inflightFences.resize(m_imageCount);
	m_imagesinflight.resize(m_imageCount, VK_NULL_HANDLE);
	
	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	
	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	
	for(std::size_t i = 0; i < m_imageCount; i++)
	{
	  VkResult f_sem = vkCreateSemaphore(m_pVtDevices->getLogicalDevice(), &semaphoreInfo, nullptr, &m_imagesAvailable[i]);
	  VkResult l_sem = vkCreateSemaphore(m_pVtDevices->getLogicalDevice(), &semaphoreInfo, nullptr, &m_rendersFinished[i]);
	  VkResult fence = vkCreateFence(m_pVtDevices->getLogicalDevice(), &fenceInfo, nullptr, &m_inflightFences[i]);

		VtUtil::checkVulkanResult(m_name+"::VtSubmitQueue::m_imagesAvailable", f_sem);
		VtUtil::checkVulkanResult(m_name+"::VtSubmitQueue::m_rendersFinished", l_sem);
		VtUtil::checkVulkanResult(m_name+"::VtSubmitQueue::m_inflightFences", fence);
	
	}
	VtLogHandler::oStream("V-Toolbox", m_name+"::VtSubmitQueue", "Success to create");
}

VtSubmitQueue::~VtSubmitQueue()
{
	if(m_imageCount != 0)
	{
		for(std::size_t i = 0; i < m_imageCount; i++)
    {
      if(m_rendersFinished[i] != VK_NULL_HANDLE)
      {
        vkDestroySemaphore(m_pVtDevices->getLogicalDevice(), m_rendersFinished[i], nullptr);
        VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::~VtSubmitQueue::rendersFinished["+std::to_string(i)+"]", "Success to destroy VkSemaphore");
      }

      if(m_inflightFences[i] != VK_NULL_HANDLE)
      {
        vkDestroySemaphore(m_pVtDevices->getLogicalDevice(), m_imagesAvailable[i], nullptr);
        VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::~VtSubmitQueue::imagesAvailable["+std::to_string(i)+"]", "Success to destroy VkSemaphore");
      }

      if(m_inflightFences[i] != VK_NULL_HANDLE)
      {
        vkDestroyFence(m_pVtDevices->getLogicalDevice(), m_inflightFences[i], nullptr);
        VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::~VtSubmitQueue::inflightFences["+std::to_string(i)+"]", "Success to destroy VkFence");
      }
    }
    m_rendersFinished.clear();
    m_imagesAvailable.clear();
    m_inflightFences.clear();
    VtLogHandler::oStream("V-Toolbox", m_name+"::~VtSubmitQueue", "Success to destroy");
	}	
}

bool VtSubmitQueue::submit(const std::vector<VkCommandBuffer>& t_commandBuffers)
{
  assert(m_pVtDevices != nullptr);
  assert(m_pVtSwapchain != nullptr);

  VkResult waitRes = vkWaitForFences(m_pVtDevices->getLogicalDevice(), 1, &m_inflightFences[m_currentFrame], VK_TRUE, UINT64_MAX);
  VtUtil::checkVulkanResult(m_name+"::VtSubmitQueue::waitRes", waitRes);

  std::uint32_t imageIndex = 0;
  VkResult imageRes = m_pVtSwapchain->acquireImage(m_imagesAvailable[m_currentFrame], imageIndex);
  assert(imageRes == VK_SUCCESS);

  if(imageRes == VK_ERROR_OUT_OF_DATE_KHR)
    return true;
  else if(imageRes != VK_SUCCESS && imageRes != VK_SUBOPTIMAL_KHR)
		VtLogHandler::oStreamFatalError("V-Toolbox", m_name+"::VtSubmitQueue::submit", "failed to acquire an image from the swapchain");

  if(m_imagesinflight[imageIndex] != VK_NULL_HANDLE)
    vkWaitForFences(m_pVtDevices->getLogicalDevice(), 1, &m_imagesinflight[imageIndex], VK_TRUE, UINT64_MAX);

  //_Semaphores_//
  VkSemaphore waitSemaphores[]      = {m_imagesAvailable[m_currentFrame]};
  VkSemaphore signalSemaphores[]    = {m_rendersFinished[m_currentFrame]};
  VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

  VkSubmitInfo submitInfo = {};
  submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount   = 1;
  submitInfo.pCommandBuffers      = &t_commandBuffers[imageIndex];
  submitInfo.waitSemaphoreCount   = 1;
  submitInfo.pWaitSemaphores      = waitSemaphores;
  submitInfo.pWaitDstStageMask    = waitStages;
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores    = signalSemaphores;

  vkResetFences(m_pVtDevices->getLogicalDevice(), 1, &m_inflightFences[m_currentFrame]);

  VkResult submitRes = vkQueueSubmit(m_pVtDevices->getQueue(0), 1, &submitInfo, m_inflightFences[m_currentFrame]);
  assert(submitRes == VK_SUCCESS);

  //_Present_//
  VkSwapchainKHR swapchains[] = {m_pVtSwapchain->getInstance()};

  VkPresentInfoKHR presentInfo = {};
  presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores    = signalSemaphores;
  presentInfo.swapchainCount     = 1;
  presentInfo.pSwapchains        = swapchains;
  presentInfo.pImageIndices      = &imageIndex;
  presentInfo.pResults           = nullptr;

  VkResult presentRes = vkQueuePresentKHR(m_pVtDevices->getQueue(0), &presentInfo);
  VtUtil::checkVulkanResult(m_name+"::VtSubmitQueue::presentRes", presentRes);

  if(presentRes == VK_ERROR_OUT_OF_DATE_KHR || presentRes == VK_SUBOPTIMAL_KHR)
    return true;
  else if(presentRes != VK_SUCCESS)
 	 VtLogHandler::oStreamFatalError("V-Toolbox", m_name+"::VtSubmitQueue::submit", "failed to present an image");

  m_currentFrame = (m_currentFrame+1) % m_imagesAvailable.size();

  return false;
}
