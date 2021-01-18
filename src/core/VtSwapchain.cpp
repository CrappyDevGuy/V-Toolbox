//_Author : CrappyDevGuy, M12D29Y2020_//

#include "core/VtSwapchain.hpp"

#include "core/io/VtLogHandler.hpp"

VtSwapchain& VtSwapchain::operator=(VtSwapchain&& other) noexcept
{
	std::swap(m_swapchain, other.m_swapchain);
	std::swap(m_pVtDevices, other.m_pVtDevices);
	std::swap(m_data, other.m_data);
	return *this;
}

VtSwapchain::VtSwapchain(VtSwapchain&& other) noexcept
:m_swapchain{std::exchange(other.m_swapchain, nullptr)}
,m_pVtDevices{std::exchange(other.m_pVtDevices, nullptr)}
,m_data{std::move(other.m_data)}
{}

VtSwapchain::VtSwapchain(VtSwapchainCreateInfo _createInfo)
{
	assert(_createInfo.pVtWindow != nullptr);	
	assert(_createInfo.pVtDevices != nullptr);	

	m_pVtDevices  = _createInfo.pVtDevices;
	m_data.name   = std::move(_createInfo.name);

	auto                      swapChainSupport    = m_pVtDevices->getSwapchainSupport();
  VkSurfaceFormatKHR        surfaceFormat       = chooseSurfaceFormat(swapChainSupport.surfaceFormats);
  VkPresentModeKHR          presentMode         = choosePresentMode(swapChainSupport.presentModes, _createInfo.presentMode);
  VkSurfaceCapabilitiesKHR  c_capabilities      = swapChainSupport.capabilities;
  VkExtent2D                extent              = chooseExtent(_createInfo.pVtWindow->getExtent(), c_capabilities);
  std::uint32_t             swapchainImageCount = getImageCount(c_capabilities);

  std::string supportedPresentMode = "";
  for(auto pmode :  swapChainSupport.presentModes)
  {
         if(pmode == VK_PRESENT_MODE_IMMEDIATE_KHR)    supportedPresentMode += "VK_PRESENT_MODE_IMMEDIATE_KHR ";
    else if(pmode == VK_PRESENT_MODE_FIFO_KHR)         supportedPresentMode += "VK_PRESENT_MODE_FIFO_KHR ";
    else if(pmode == VK_PRESENT_MODE_FIFO_RELAXED_KHR) supportedPresentMode += "VK_PRESENT_MODE_FIFO_RELAXED_KHR ";
    else if(pmode == VK_PRESENT_MODE_MAILBOX_KHR)      supportedPresentMode += "VK_PRESENT_MODE_MAILBOX_KHR ";
  }

	VtLogHandler::oStreamDebug("V-Toolbox", m_data.name+"::VtSwapchain", "supported Present Mode : " + supportedPresentMode);

  VkSwapchainCreateInfoKHR createInfo = {};
  createInfo.sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.minImageCount         = swapchainImageCount;
  createInfo.surface               = _createInfo.pVtWindow->getSurfaceInstance();
  createInfo.imageExtent           = extent;
  createInfo.imageUsage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  createInfo.imageFormat           = surfaceFormat.format;
  createInfo.imageColorSpace       = surfaceFormat.colorSpace;
  createInfo.imageArrayLayers      = 1;
  createInfo.queueFamilyIndexCount = 0;
  createInfo.pQueueFamilyIndices   = nullptr;
  createInfo.preTransform          = c_capabilities.currentTransform;
  createInfo.compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.presentMode           = presentMode;
  createInfo.clipped               = VK_TRUE;
  createInfo.oldSwapchain          = VK_NULL_HANDLE;

  std::uint32_t presentQueueIndex  = _createInfo.pVtDevices->getQueueIndex(0);
  std::uint32_t graphicsQueueIndex = _createInfo.pVtDevices->getQueueIndex(0);

  if(presentQueueIndex != graphicsQueueIndex)
  {
    std::uint32_t queueFamilyIndices[] = {presentQueueIndex, graphicsQueueIndex};
    createInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices   = queueFamilyIndices;
  }else
    createInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;

  auto logicalDevice = _createInfo.pVtDevices->getLogicalDevice();

  VtUtil::checkVulkanResult(m_data.name+"::VtSwapchain::Create", vkCreateSwapchainKHR(logicalDevice, &createInfo, nullptr, &m_swapchain));

  std::vector<VkImage> a_images;
  VtUtil::checkVulkanResult(m_data.name+"::VtSwapchain::ImagesCount", vkGetSwapchainImagesKHR(logicalDevice, m_swapchain, &swapchainImageCount, nullptr));
  a_images.resize(swapchainImageCount);
  VtUtil::checkVulkanResult(m_data.name+"::VtSwapchain::ImagesView", vkGetSwapchainImagesKHR(logicalDevice, m_swapchain, &swapchainImageCount, a_images.data()));

  m_data.imagesView.resize(swapchainImageCount);
  for(auto i = 0u; i < m_data.imagesView.size(); i++)
  {
    VtImageCreateInfo createInfo = {};
    createInfo.name          = "SwapchainImages[" + std::to_string(i) + "]";
    createInfo.pVtDevices    = m_pVtDevices;
    createInfo.format        = surfaceFormat.format;

    m_data.imagesView[i] = {std::move(createInfo)};
    m_data.imagesView[i].setImage(std::move(a_images[i]));
  }

  m_data.imageFormat = surfaceFormat.format;
  m_data.extent      = {extent.width, extent.height};

  createImageViews();
  createImages();
	VtLogHandler::oStream("V-Toolbox", m_data.name+"::VtSwapchain", "Success to create, Images Count = " + std::to_string(swapchainImageCount));
}

VtSwapchain::~VtSwapchain()
{
	if(m_swapchain != nullptr)
	{
    vkDestroySwapchainKHR(m_pVtDevices->getLogicalDevice(), m_swapchain, nullptr);
		m_swapchain = nullptr;
		VtLogHandler::oStream("V-Toolbox", m_data.name+"::~VtSwapchain", "Success to destroy VkSwapchainKHR");
	}
}

VkResult VtSwapchain::acquireImage(VkSemaphore& t_semaphore, std::uint32_t& imageIndex)
{
  assert(m_swapchain != nullptr);

	VkResult res = vkAcquireNextImageKHR(m_pVtDevices->getLogicalDevice(), m_swapchain, UINT64_MAX, t_semaphore, VK_NULL_HANDLE, &imageIndex);
  VtUtil::checkVulkanResult(m_data.name+"::VtSwapchain::acquireImage", res);
  return res;
}

//_ Private Functions _//
void VtSwapchain::createImageViews()
{
  for(auto i = 0u; i < m_data.imagesView.size(); i++)
  {
    m_data.imagesView[i].createView(VK_IMAGE_ASPECT_COLOR_BIT);
  }

	VtLogHandler::oStreamDebug("V-Toolbox", m_data.name+"::VtSwapchain::createImageViews", "Success to create");
}

void VtSwapchain::createImages()
{
  std::vector<VkFormat> candidates = {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT};
  m_data.depthFormat = m_pVtDevices->getSupportedFormat(candidates, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT, "SwapChainDepthFormat");

  VtImageCreateInfo depth_createInfo = {};
  depth_createInfo.name          = m_data.name+"_DepthImage";
  depth_createInfo.pVtDevices    = m_pVtDevices;
  depth_createInfo.format        = m_data.depthFormat;

  VtImageInfo depth_imageInfo   = {};
  depth_imageInfo.extent        = m_data.extent;
  depth_imageInfo.createMipMaps = false;
  depth_imageInfo.tiling        = VK_IMAGE_TILING_OPTIMAL;
  depth_imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  depth_imageInfo.usage         = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
  depth_imageInfo.samplesCount  = m_pVtDevices->getSamplesCount();

  m_data.depthImage = {depth_createInfo};
  m_data.depthImage.createImage(std::move(depth_imageInfo));
  m_data.depthImage.createMemory(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  m_data.depthImage.createView(VK_IMAGE_ASPECT_DEPTH_BIT);

  VtImageCreateInfo color_createInfo = {};
  color_createInfo.name          = m_data.name+"_ColorImage";
  color_createInfo.pVtDevices    = m_pVtDevices;
  color_createInfo.format        = m_data.imageFormat;

  VtImageInfo color_imageInfo = {};
  color_imageInfo.extent        = m_data.extent;
  color_imageInfo.createMipMaps = false;
  color_imageInfo.tiling        = VK_IMAGE_TILING_OPTIMAL;
  color_imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  color_imageInfo.usage         = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  color_imageInfo.samplesCount  = m_pVtDevices->getSamplesCount();

  m_data.colorImage = {std::move(color_createInfo)};
  m_data.colorImage.createImage(std::move(color_imageInfo));
  m_data.colorImage.createMemory(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  m_data.colorImage.createView(VK_IMAGE_ASPECT_COLOR_BIT);
}

std::uint32_t VtSwapchain::getImageCount(const VkSurfaceCapabilitiesKHR& capabilities)
{
  std::uint32_t imageCount = capabilities.minImageCount+1;
  std::uint32_t maxImageCount = capabilities.maxImageCount;

  if(maxImageCount > 0 && imageCount > maxImageCount)
    return maxImageCount;

  return imageCount;
}

VkSurfaceFormatKHR VtSwapchain::chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availables)
{
  for(const auto& a_format : availables)
    if(a_format.format == VK_FORMAT_B8G8R8A8_UNORM && a_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
      return a_format;

  return availables[0];
}

VkPresentModeKHR VtSwapchain::choosePresentMode(const std::vector<VkPresentModeKHR>& availables, VkPresentModeKHR t_present)
{
  for(const auto& a_present : availables)
    if(a_present == t_present)
      return a_present;
    else if(a_present == VK_PRESENT_MODE_MAILBOX_KHR)
      return a_present;

  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VtSwapchain::chooseExtent(glm::ivec2 t_extent, const VkSurfaceCapabilitiesKHR& capabilities)
{
  if(capabilities.currentExtent.width != UINT32_MAX)
    return capabilities.currentExtent;
  else
  {
    VkExtent2D a_extent = {
  	  static_cast<std::uint32_t>(t_extent.x),
      static_cast<std::uint32_t>(t_extent.y)
    };

    a_extent.width  = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, a_extent.width));
    a_extent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, a_extent.height));
    return a_extent;
  }
}
