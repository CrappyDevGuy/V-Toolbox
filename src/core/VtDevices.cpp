//_Author : CrappyDevGuy, M12D29Y2020_//

#include "core/VtDevices.hpp"

#include "core/io/VtLogHandler.hpp"
#include "core/util/VtUtil.hpp"

#include <set>

VtDevices& VtDevices::operator=(VtDevices&& other) noexcept
{
	std::swap(m_data, other.m_data);
	std::swap(m_logicalDevice, other.m_logicalDevice);
	std::swap(m_physicalDevice, other.m_physicalDevice);
	return *this;
}

VtDevices::VtDevices(VtDevices&& other) noexcept
:m_data{std::move(other.m_data)}
,m_logicalDevice{std::exchange(other.m_logicalDevice, nullptr)}
,m_physicalDevice{std::exchange(other.m_physicalDevice, nullptr)}
{}

VtDevices::VtDevices(VtDevicesCreateInfo _createInfo)
{
	assert(_createInfo.pVtWindow != nullptr);

	m_data.name = std::move(_createInfo.name);

	buildPhysicalDevice(_createInfo.physicalDevices, _createInfo.pVtWindow, _createInfo.msaa);
	buildLogicalDevice(_createInfo.features);

	VtLogHandler::oStream("V-Toolbox", m_data.name+"::VtDevices", "Success to create");
	
}

VtDevices::~VtDevices()
{
	if(m_logicalDevice != nullptr)
	{
    vkDestroyDevice(m_logicalDevice, nullptr);
    m_logicalDevice = nullptr;
    VtLogHandler::oStream("V-Toolbox", m_data.name+"::~VtDevices", "Success to destroy VkDevice");
	}
}

std::uint32_t VtDevices::findMemoryType(std::uint32_t typeFilter, VkMemoryPropertyFlags properties, std::string debugname)
{

  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProperties);

  for(std::uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    if(typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
      return i;

	VtLogHandler::oStreamFatalError("V-Toolbox", m_data.name+"::VtDevices::findMemoryType", "failed to find a memory match, debugNameRequest : " + debugname);
  return -1;
}

VkFormat VtDevices::getSupportedFormat(std::vector<VkFormat> candidates, VkImageTiling tiling, VkFormatFeatureFlags features, std::string debugname)
{
  for(auto format : candidates)
  {
    VkFormatProperties properties;
    vkGetPhysicalDeviceFormatProperties(m_physicalDevice, format, &properties);

    if(tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & features) == features)
      return format;
    else if(tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & features) == features)
      return format;
  }
  
  VtLogHandler::oStreamFatalError("V-Toolbox", m_data.name+"::VtDevices::getSupportedFormat", "no format match with the candidates, debugNameRequest : " + debugname);
  return candidates[0];
}

VkQueue VtDevices::getQueueByFlags(VkQueueFlags flags, bool fullmatch)
{
	VkQueue queueFound = nullptr;
  for(auto i = 0u; i < m_data.queuesData.size(); i++)
  {
    if(fullmatch)
    {
      std::vector<VkQueueFlags> c_flags = m_data.queuesData[i].enumerateFlags();


      if(c_flags[0] == flags)
        queueFound = m_data.queuesData[i].queue;

    }else
      if(m_data.queuesData[i].contains(flags))
        queueFound = m_data.queuesData[i].queue;
    
  }
	if(queueFound == nullptr)
	{
	  std::string strflags = VtUtil::VkQueueFlagsToString(flags);
		VtLogHandler::oStreamWarning("V-Toolbox", m_data.name+"::VtDevices::getQueueByFlags", "couldn't find any queue with the flag : " + strflags);
		queueFound = m_data.queuesData[0].queue;	
	}

  return queueFound;
}

std::uint32_t VtDevices::getQueueIndexByFlags(VkQueueFlags flags, bool fullmatch)
{
  for(auto i = 0u; i < m_data.queuesData.size(); i++)
  {
    if(fullmatch)
    {
      std::vector<VkQueueFlags> c_flags = m_data.queuesData[i].enumerateFlags();

      if(c_flags[0] == flags)
        return m_data.queuesData[i].index;

    }else
      if(m_data.queuesData[i].contains(flags))
        return m_data.queuesData[i].index;   
  }
  std::string strflags = VtUtil::VkQueueFlagsToString(flags);

 	VtLogHandler::oStreamWarning("V-Toolbox", m_data.name+"::VtDevices::getQueueIndexByFlags", "couldn't find any queue with the flag : " + strflags);
  return m_data.queuesData[0].index;
}

//_ Private Functions _//
void VtDevices::buildLogicalDevice(VkPhysicalDeviceFeatures features)
{
  assert(m_physicalDevice != nullptr);

  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
  std::vector<std::uint32_t> queueFamilies;

  for(auto c_queue : m_data.queuesData)
  {
    queueFamilies.push_back(c_queue.index);
  }
  
  m_data.queuesData.resize(queueFamilies.size());
  
  auto queuePriority = 1.0f;
  for(std::uint32_t queueFamily : queueFamilies)
  {
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex        = queueFamily;
    queueCreateInfo.queueCount              = 1;
    queueCreateInfo.pQueuePriorities        = &queuePriority;
    queueCreateInfos.push_back(queueCreateInfo);
  }

  VkDeviceCreateInfo createInfo = {};
  createInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.pEnabledFeatures        = &features;

  createInfo.pQueueCreateInfos       = queueCreateInfos.data();
  createInfo.queueCreateInfoCount    = static_cast<std::uint32_t>(queueCreateInfos.size());

  createInfo.ppEnabledExtensionNames = VtDeviceExtensions.data();
  createInfo.enabledExtensionCount   = static_cast<std::uint32_t>(VtDeviceExtensions.size());


  VtUtil::checkVulkanResult(m_data.name+"::VtDevices::vkCreateDevice", vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_logicalDevice));

  volkLoadDevice(m_logicalDevice);
  
  for(unsigned int i = 0; i < m_data.queuesData.size(); i++)
  {
    vkGetDeviceQueue(m_logicalDevice, queueFamilies[i], 0, &m_data.queuesData[i].queue);
  }
  
	VtLogHandler::oStreamDebug("V-Toolbox", m_data.name+"::VtDevices::buildLogicalDevice", "Success to create");
}

void VtDevices::buildPhysicalDevice(std::vector<VkPhysicalDevice> devices, VtWindow* pVtWindow, VkSampleCountFlagBits msaa)
{
	for(const auto& a_device : devices)
    if(isDeviceSuitable(a_device, pVtWindow))
    {
      m_physicalDevice = a_device;
      break;
    }

  if(m_physicalDevice == nullptr)
  	VtLogHandler::oStreamFatalError("V-Toolbox", m_data.name+"::VtDevices::buildPhysicalDevice", "none of the graphics card found can run this program");

  VkPhysicalDeviceProperties deviceproperties;
  vkGetPhysicalDeviceProperties(m_physicalDevice, &deviceproperties);

  VkSampleCountFlags counts = deviceproperties.limits.framebufferColorSampleCounts & deviceproperties.limits.framebufferDepthSampleCounts;


       if (counts & msaa)                   { m_data.msaaSamples = msaa;                   }
  else if (counts & VK_SAMPLE_COUNT_64_BIT) { m_data.msaaSamples = VK_SAMPLE_COUNT_64_BIT; }
  else if (counts & VK_SAMPLE_COUNT_32_BIT) { m_data.msaaSamples = VK_SAMPLE_COUNT_32_BIT; }
  else if (counts & VK_SAMPLE_COUNT_16_BIT) { m_data.msaaSamples = VK_SAMPLE_COUNT_16_BIT; }
  else if (counts & VK_SAMPLE_COUNT_8_BIT)  { m_data.msaaSamples = VK_SAMPLE_COUNT_8_BIT;  }
  else if (counts & VK_SAMPLE_COUNT_4_BIT)  { m_data.msaaSamples = VK_SAMPLE_COUNT_4_BIT;  }
  else if (counts & VK_SAMPLE_COUNT_2_BIT)  { m_data.msaaSamples = VK_SAMPLE_COUNT_2_BIT;  }

  std::string gpuName = deviceproperties.deviceName;
  VtLogHandler::oStreamDebug("V-Toolbox", m_data.name+"::VtDevices::buildPhysicalDevice", "graphics card found { " + gpuName + " }");
}

bool VtDevices::isDeviceSuitable(VkPhysicalDevice device, VtWindow* pVtWindow)
{
  m_data.queuesData.clear();
  findAllQueues(device, m_data.queuesData);

  // check if the gpu at least support the main flags
  std::uint32_t queueIndex       = 0;
  VkBool32      deviceQueueMatch = false;
  for(auto& c_queue : m_data.queuesData)
  {
    deviceQueueMatch = c_queue.contains(VK_QUEUE_GRAPHICS_BIT);

    vkGetPhysicalDeviceSurfaceSupportKHR(device, queueIndex, pVtWindow->getSurfaceInstance(), &c_queue.presentSupport);

    if(deviceQueueMatch && c_queue.presentSupport) break;

    queueIndex++;
  }

  bool extensionsSupport = checkDeviceExtensionSupport(device);

  VkPhysicalDeviceFeatures supportedFeatures;
  vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

  bool swapChainCorrect = false;
  if(extensionsSupport)
  {
    auto swapChainSupport   = findSwapChainSupport(device, pVtWindow->getSurfaceInstance());
    swapChainCorrect        = !swapChainSupport.surfaceFormats.empty() && !swapChainSupport.presentModes.empty();
    m_data.swapchainSupport = std::move(swapChainSupport);
  }

  return deviceQueueMatch && extensionsSupport && swapChainCorrect && supportedFeatures.samplerAnisotropy;
}

void VtDevices::findAllQueues(VkPhysicalDevice device, std::vector<VtDevicesVkQueueData>& queuesData)
{
  std::uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

  VkPhysicalDeviceProperties deviceproperties;
  vkGetPhysicalDeviceProperties(device, &deviceproperties);

  std::string gpuName = deviceproperties.deviceName;

  VtLogHandler::oStreamDebug("V-Toolbox", m_data.name+"::VtDevices::findAllQueues", "Finding all queues, GPU : " + gpuName);

	queuesData.resize(queueFamilyCount);
  for(std::uint32_t i = 0; i < queueFamilyCount; i++)
  {
    queuesData[i].flags = queueFamilies[i].queueFlags;
    queuesData[i].count = queueFamilies[i].queueCount;
    queuesData[i].index = i;
    VtLogHandler::oStreamDebug("V-Toolbox", m_data.name+"::VtDevices::findAllQueues", queuesData[i].to_string());
  }
}

bool VtDevices::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
  std::uint32_t extensionCount = 0;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

  if(extensionCount == 0)
    VtLogHandler::oStreamError("V-Toolbox", m_data.name+"::VtDevices::checkDeviceExtensionSupport", "no extension found");
  else
    VtLogHandler::oStreamDebug("V-Toolbox", m_data.name+"::VtDevices::checkDeviceExtensionSupport", std::to_string(extensionCount) + " extension found");

  std::set<std::string> requiredExtensions(VtDeviceExtensions.begin(), VtDeviceExtensions.end());

  for(const auto& a_ext : availableExtensions)
    requiredExtensions.erase(a_ext.extensionName);

  return requiredExtensions.empty();
}

VtDevicesSwapChainSupportDetails VtDevices::findSwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
{
  VtDevicesSwapChainSupportDetails details;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

  std::uint32_t formatCount = 0;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

  if(formatCount != 0)
  {
    details.surfaceFormats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.surfaceFormats.data());
    VtLogHandler::oStreamDebug("V-Toolbox", m_data.name+"::VtDevices::foundSwapChainSupport", std::to_string(formatCount) + " surface formats found");
  }else
    VtLogHandler::oStreamError("V-Toolbox", m_data.name+"::VtDevices::foundSwapChainSupport", " no surface formats found");

  std::uint32_t presentCount = 0;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentCount, nullptr);

  if(presentCount != 0)
  {
    details.presentModes.resize(presentCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentCount, details.presentModes.data());
    VtLogHandler::oStreamDebug("V-Toolbox", m_data.name+"::VtDevices::foundSwapChainSupport", std::to_string(presentCount) + " present modes found");
	}else
    VtLogHandler::oStreamError("V-Toolbox", m_data.name+"::VtDevices::foundSwapChainSupport", " no present modes found");

  return details;
}
