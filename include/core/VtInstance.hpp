//_Author : CrappyDevGuy, M12D08Y2020_//

#pragma once

#include "volk.h"
#include "io/VtLogHandler.hpp"
#include <utility>
#include <vector>

static const std::vector<const char*> VtValidationLayers = { "VK_LAYER_KHRONOS_validation" };
static const std::vector<const char*> VtDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

  
struct VtInstanceCreateInfo
{
	std::string   appName       = "Undefined";
	std::uint32_t appVersion    = VK_MAKE_VERSION(1,0,0);
};

class VtInstance
{

	public:
		VtInstance(VtInstanceCreateInfo _createInfo);
		VtInstance(const VtInstance&) = delete;
		VtInstance() = default;
		~VtInstance();

		VtInstance& operator=(const VtInstance&) = delete;
		VtInstance& operator=(VtInstance&& other) noexcept;
		VtInstance(VtInstance&& other) noexcept;

		auto& getVkInstance() 		 const { return m_instance; };
		auto& getVkCallback() 		 const { return m_callback; };
		std::string_view getName() const { return m_name;     };

	private:
		bool 									   checkValidationLayerSupport();
		std::vector<const char*> getRequiredExtensions(bool validationLayers);

	private:
		VkInstance  						 m_instance = nullptr;
		VkDebugUtilsMessengerEXT m_callback = nullptr;
		std::string 						 m_name     = "Undefined";
};
