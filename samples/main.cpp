//_Author : CrappyDevGuy, M12D08Y2020_//

#include "core/io/VtLogHandler.hpp"

#include "core/VtInstance.hpp"
#include "core/VtWindow.hpp"
#include "core/VtDevices.hpp"
#include "core/VtSwapchain.hpp"
#include "core/graphics/pool/VtCommandPool.hpp"
#include "core/graphics/pool/VtDescriptorPool.hpp"
#include "core/graphics/VtCommandBuffer.hpp"
#include "core/graphics/VtShader.hpp"

#include <iostream>
#include <iomanip>

int main()
{

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

	VtCommandPoolCreateInfo commandPoolCreateInfo{};
	commandPoolCreateInfo.name  					 = "MainCommandPool";
	commandPoolCreateInfo.flags 					 = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	commandPoolCreateInfo.queueFamilyIndex = devices.getQueueIndex(0);
	commandPoolCreateInfo.pVtDevices			 = &devices;

	VtCommandPool commandPool{commandPoolCreateInfo};

	std::vector<VtCommandBuffer> commandBuffers(3);

	VtCommandBufferCreateInfo commandBufferCreateInfo{};
  commandBufferCreateInfo.pVtDevices = &devices;

	for(auto i = 0u; i < commandBuffers.size(); i++)
	{
		commandBufferCreateInfo.name  = "CommandBuffer[" + std::to_string(i) + "]";

		commandBuffers[i] = {commandBufferCreateInfo};
		commandBuffers[i].alloc(&commandPool);
	}

	VtDescriptorPoolCreateInfo descriptorPoolCreateInfo{};
	descriptorPoolCreateInfo.name       = "MainDescriptorPool";
	descriptorPoolCreateInfo.setsCount  = 1;
	descriptorPoolCreateInfo.pVtDevices = &devices;
	descriptorPoolCreateInfo.poolsData  = {
		{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1}
	};

	VtDescriptorPool descriptorPool{descriptorPoolCreateInfo};
	
  VtLogHandler::oStream("myproject", "main", "Starting");

	while(!window.shouldClose())
	{
		window.update();
	}
	
	VtLogHandler::oStream("myproject", "main", "Ending");
	return 0;
}
