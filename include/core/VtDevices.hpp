//_Author : CrappyDevGuy, M12D29Y2020_//

#pragma once


#include "core/VtWindow.hpp"
#include "core/util/VtUtil.hpp"

struct VtDevicesVkQueueData
{
	VkQueue       queue = nullptr;
  VkQueueFlags  flags;
  std::uint32_t count,
                index;
  VkBool32      presentSupport = false;

  std::string to_string()
  {
    std::string strflags = VtUtil::VkQueueFlagsToString(flags);
    strflags.erase(strflags.end()-2, strflags.end());
    return std::to_string(index)+" : Count{"+std::to_string(count)+"}, Flags{"+strflags+"}";
  };

  bool contains(VkQueueFlags currentflags)
  {
    return flags & currentflags;
  };

  bool match(VkQueueFlags currentflags)
  {
    return flags == currentflags;
  };

  std::vector<VkQueueFlags> enumerateFlags()
  {
    std::vector<VkQueueFlags> c_flags;
    if(flags & VK_QUEUE_GRAPHICS_BIT)       c_flags.push_back(VK_QUEUE_GRAPHICS_BIT);
    if(flags & VK_QUEUE_TRANSFER_BIT)       c_flags.push_back(VK_QUEUE_TRANSFER_BIT);
    if(flags & VK_QUEUE_COMPUTE_BIT)        c_flags.push_back(VK_QUEUE_COMPUTE_BIT);
    if(flags & VK_QUEUE_SPARSE_BINDING_BIT) c_flags.push_back(VK_QUEUE_SPARSE_BINDING_BIT);
    if(flags & VK_QUEUE_PROTECTED_BIT)      c_flags.push_back(VK_QUEUE_PROTECTED_BIT);
    return c_flags;
  }
};

struct VtDevicesSwapChainSupportDetails
{
  VkSurfaceCapabilitiesKHR        capabilities;
  std::vector<VkSurfaceFormatKHR> surfaceFormats;
  std::vector<VkPresentModeKHR>   presentModes;
};

struct VtDevicesCreateInfo
{
	std::string name{"NULL"};
  VkPhysicalDeviceFeatures features;
  VkSampleCountFlagBits    msaa;

  std::vector<VkPhysicalDevice> physicalDevices;
	VtWindow* pVtWindow{nullptr};
};

class VtDevices
{
	struct InternalData
	{
		std::string name;

		std::vector<VtDevicesVkQueueData> queuesData;
    VkSampleCountFlagBits             msaaSamples{VK_SAMPLE_COUNT_1_BIT};
    VtDevicesSwapChainSupportDetails  swapchainSupport;
	};
	
	public:
		VtDevices(VtDevicesCreateInfo _createInfo);
		VtDevices(const VtDevices&) = delete;
		VtDevices() = default;
		~VtDevices();

		VtDevices& operator=(const VtDevices&) = delete;
		VtDevices& operator=(VtDevices&& other) noexcept;
		VtDevices(VtDevices&& other) noexcept;

    inline std::uint32_t 										getQueueIndex(std::uint32_t index) { return m_data.queuesData[index].index; };
    inline VkQueue&      									  getQueue(std::uint32_t index)      { return m_data.queuesData[index].queue; };
    inline VtDevicesSwapChainSupportDetails getSwapchainSupport() 						 { return m_data.swapchainSupport;        };

		inline auto& getLogicalDevice()  const noexcept { return m_logicalDevice;				   };
		inline auto& getPhysicalDevice() const noexcept { return m_physicalDevice; 				 };
    inline auto  getSamplesCount()   const noexcept { return m_data.msaaSamples; 			 };

    inline void waitDevice() { vkDeviceWaitIdle(m_logicalDevice); };
		
    std::uint32_t findMemoryType(std::uint32_t typeFilter, VkMemoryPropertyFlags properties, std::string debugname);
		VkFormat      getSupportedFormat(std::vector<VkFormat> candidates, VkImageTiling tiling, VkFormatFeatureFlags features, std::string debugname = "NULL");

    VkQueue       getQueueByFlags(VkQueueFlags flags, bool fullmatch);
    std::uint32_t getQueueIndexByFlags(VkQueueFlags flags, bool fullmatch);
    
	private:
		void buildLogicalDevice(VkPhysicalDeviceFeatures features);
		void buildPhysicalDevice(std::vector<VkPhysicalDevice> devices, VtWindow* pVtWindow, VkSampleCountFlagBits msaa);

		bool isDeviceSuitable(VkPhysicalDevice device, VtWindow* pVtWindow);
		void findAllQueues(VkPhysicalDevice device, std::vector<VtDevicesVkQueueData>& queuesData);
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		VtDevicesSwapChainSupportDetails findSwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
		
	private:

  	VkDevice         m_logicalDevice{nullptr};
  	VkPhysicalDevice m_physicalDevice{nullptr};
  
		VtDevices::InternalData m_data{};
};
