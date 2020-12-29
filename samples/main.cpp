//_Author : CrappyDevGuy, M12D08Y2020_//

#include "core/io/VtLogHandler.hpp"

#include "core/VtInstance.hpp"
#include "core/VtWindow.hpp"
#include "core/VtDevices.hpp"
#include "core/VtSwapchain.hpp"

#include <iostream>
#include <iomanip>

int main()
{

	// VtLogHandler::oStreamDebug("myproject", "TEST123", "ABC");
	// VtLogHandler::oStream("myproject", "TEST123", "ABC");
	// VtLogHandler::oStreamError("myproject", "TEST123", "ABC");
	// VtLogHandler::oStreamWarning("myproject", "TEST123", "ABC");
	// VtLogHandler::oStreamFatalError("myproject", "TEST123", "ABC");

	VtInstanceCreateInfo instanceCreateInfo{};
	instanceCreateInfo.appName    = "MainInstance";
	instanceCreateInfo.appVersion = VK_MAKE_VERSION(0,0,1);

	VtInstance instance{instanceCreateInfo};

	VtWindowCreateInfo windowCreateInfo{};
	windowCreateInfo.pVtInstance = &instance;
	windowCreateInfo.name        = "MainWindow";
	windowCreateInfo.extent      = {800, 600};
	windowCreateInfo.windowhints = {
		{GLFW_CLIENT_API, GLFW_NO_API},
		{GLFW_RESIZABLE , GLFW_FALSE}
	};

	VtWindow window{windowCreateInfo};
	
  VkPhysicalDeviceFeatures features = {};
  features.samplerAnisotropy = VK_TRUE;
  features.fillModeNonSolid  = VK_TRUE;

	VtDevicesCreateInfo devicesCreateInfo{};
	devicesCreateInfo.name     			  = "MainDevices";
	devicesCreateInfo.msaa      			= VK_SAMPLE_COUNT_4_BIT;
	devicesCreateInfo.features  			= std::move(features);
	devicesCreateInfo.physicalDevices = instance.enumeratePhysicalDevices();
	devicesCreateInfo.pVtWindow 			= &window;

	VtDevices devices{devicesCreateInfo};

	VtSwapchainCreateInfo swapchainCreateInfo{};
	swapchainCreateInfo.name        = "MainSwapchain";
	swapchainCreateInfo.pVtWindow   = &window;
	swapchainCreateInfo.pVtDevices  = &devices;
	swapchainCreateInfo.presentMode = VK_PRESENT_MODE_MAILBOX_KHR;

	VtSwapchain swapchain{swapchainCreateInfo};

  VtLogHandler::oStream("myproject", "main", "Starting");

	while(!window.shouldClose())
	{
		window.update();
	}
	
	VtLogHandler::oStream("myproject", "main", "Ending");
	return 0;
}
