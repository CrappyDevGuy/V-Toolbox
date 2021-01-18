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
#include "core/graphics/VtPipeline.hpp"
#include "core/graphics/layout/VtPipelineLayout.hpp"
#include "core/graphics/layout/VtDescriptorLayout.hpp"
#include "core/graphics/VtFramebuffer.hpp"
#include "core/graphics/VtDescriptorSet.hpp"
#include "core/graphics/VtBuffer.hpp"

#include <iostream>
#include <iomanip>

constexpr static glm::ivec2 appExtent{800, 600};

struct ViewMatrices
{
	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;
};

struct ModelMatrices
{
	glm::mat4 transformMatrix;
};

struct Material
{
	glm::vec4 ambient;
	glm::vec4 diffuse;
};

int main()
{

	VtInstanceCreateInfo instanceCreateInfo{};
	instanceCreateInfo.appName    = "MainInstance";
	instanceCreateInfo.appVersion = VK_MAKE_VERSION(0,0,1);

	VtInstance instance{instanceCreateInfo};

	VtWindowCreateInfo windowCreateInfo{};
	windowCreateInfo.pVtInstance = &instance;
	windowCreateInfo.name        = "MainWindow";
	windowCreateInfo.extent      = appExtent;
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
	descriptorPoolCreateInfo.setsCount  = 4;
	descriptorPoolCreateInfo.pVtDevices = &devices;
	descriptorPoolCreateInfo.poolsData  = {
		{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 10}
	};

	VtDescriptorPool descriptorPool{descriptorPoolCreateInfo};

	VtShaderInput shaderInput{};
	shaderInput.addInput(0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0);
	shaderInput.addBinding(0, VK_VERTEX_INPUT_RATE_VERTEX, 3*sizeof(float));

	VtShaderCreateInfo shaderCreateInfo{};
	shaderCreateInfo.name        = "MainShader";
	shaderCreateInfo.vertShader  = "../samples/res/shaders/shader.vert.spv";
	shaderCreateInfo.fragShader  = "../samples/res/shaders/shader.frag.spv";
	shaderCreateInfo.pVtDevices  = &devices;
	shaderCreateInfo.shaderInput = shaderInput;

	VtShader mainShader{shaderCreateInfo};

	VtSubpass mainSubpass{"MainSubpass"};
	
	VtSubpassDescription subpassDescription{};
	subpassDescription.name = "DescriptionMainSubpass[0]";
	subpassDescription.setPipelineBindPoint(VK_PIPELINE_BIND_POINT_GRAPHICS);
	subpassDescription.addColorReference({0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL});
	subpassDescription.addDepthReference({1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL});
	subpassDescription.addResolveReference({2, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL});
	mainSubpass.addDescription(subpassDescription);

	std::vector<VtRenderpassAttachmentInfo> renderpassAttachments(3);
	renderpassAttachments[0].format  	   = swapchain.getImageFormat();
	renderpassAttachments[0].samples 	   = devices.getSamplesCount();
	renderpassAttachments[0].storeOp 	   = VK_ATTACHMENT_STORE_OP_STORE;
	renderpassAttachments[0].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	
	renderpassAttachments[1].format  	   = swapchain.getDepthFormat();
	renderpassAttachments[1].samples 	   = devices.getSamplesCount();
	renderpassAttachments[1].storeOp 	   = VK_ATTACHMENT_STORE_OP_STORE;
	renderpassAttachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	renderpassAttachments[2].format  	   = swapchain.getImageFormat();
	renderpassAttachments[2].storeOp 	   = VK_ATTACHMENT_STORE_OP_STORE;
	renderpassAttachments[2].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	
	VtRenderpassCreateInfo renderpassCreateInfo{};
	renderpassCreateInfo.name 		   = "MainRenderPass";
	renderpassCreateInfo.pVtDevices  = &devices;
	renderpassCreateInfo.pVtSubpass  = &mainSubpass;
	renderpassCreateInfo.attachments = renderpassAttachments;

	VtRenderpass mainRenderpass{renderpassCreateInfo};

	VtDescriptorLayoutCreateInfo descriptorLayoutCreateInfo{};
	descriptorLayoutCreateInfo.name       = "MainDescriptorLayout";
	descriptorLayoutCreateInfo.pVtDevices = &devices;

	VtDescriptorLayout mainDescriptorLayout{descriptorLayoutCreateInfo};
	
	descriptorLayoutCreateInfo.name       = "ModelDescriptorLayout";
	VtDescriptorLayout modelDescriptorLayout{descriptorLayoutCreateInfo};

	mainDescriptorLayout.addBinding({0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT});
	mainDescriptorLayout.build();

	modelDescriptorLayout.addBindings({
		{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT},
		{1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT}
	});
	modelDescriptorLayout.build();

	VtPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
	pipelineLayoutCreateInfo.name       = "MainPipelineLayout";
	pipelineLayoutCreateInfo.pVtDevices = &devices;

	VtPipelineLayout mainPipelinelayout{pipelineLayoutCreateInfo};
	mainPipelinelayout.addDescriptorLayout(&mainDescriptorLayout);
	mainPipelinelayout.addDescriptorLayout(&modelDescriptorLayout);
	mainPipelinelayout.build();

	VtPipelineCreateInfo pipelineCreateInfo{};
	pipelineCreateInfo.name       = "MainPipeline";
	pipelineCreateInfo.pVtDevices = &devices;

	VtPipeline mainPipeline{pipelineCreateInfo};
	mainPipeline.setCullFace(VK_CULL_MODE_NONE);
	mainPipeline.setSamples(devices.getSamplesCount());
  mainPipeline.build(&mainShader, &mainRenderpass, &mainPipelinelayout);

	VtFramebufferCreateInfo framebufferCreateInfo{};
	framebufferCreateInfo.pVtDevices    = &devices;
	framebufferCreateInfo.pVtRenderpass = &mainRenderpass;
	framebufferCreateInfo.extent				= appExtent;
  
	std::vector<VtFramebuffer> framebuffers(swapchain.getImageCount());

	for(auto i=0u; i < framebuffers.size(); i++)
	{
		framebufferCreateInfo.name       = "Framebuffer["+std::to_string(i) + "]";
		framebufferCreateInfo.imagesView = {
			swapchain.getColorImage().getImageView(),
			swapchain.getDepthImage().getImageView(),
			swapchain.getImageView(i)
		};
			
		framebuffers[i] = {framebufferCreateInfo};
	}

	VtBuffer mainViewUB{{"mainViewUB", &devices, sizeof(ViewMatrices), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT}};
	VtBuffer modelTransformUB{{"ModelTransformUBO", &devices, sizeof(ModelMatrices), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT}};
	VtBuffer modelMaterialUB{{"modelMaterialUB", &devices, sizeof(Material), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT}};

	VtDescriptorSet mainDescriptorSet{{"MainDescriptor", &devices}};
	mainDescriptorSet.addBinding({0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER}, mainViewUB.getDescriptorSetInfo());
	mainDescriptorSet.build(&descriptorPool, &mainDescriptorLayout);
	
	std::vector<VtDescriptorSet> modelDescriptorSets(swapchain.getImageCount());
	
	VtDescriptorSetCreateInfo descriptorSetCreateInfo{};
	descriptorSetCreateInfo.pVtDevices = &devices;
	for(auto i=0u; i < modelDescriptorSets.size(); i++)
	{
		descriptorSetCreateInfo.name = "DescriptorSet[" + std::to_string(i) + "]";
		modelDescriptorSets[i] = {descriptorSetCreateInfo};
		modelDescriptorSets[i].addBinding({0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER}, modelTransformUB.getDescriptorSetInfo());
	  modelDescriptorSets[i].addBinding({1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER}, modelMaterialUB.getDescriptorSetInfo());
		
		modelDescriptorSets[i].build(&descriptorPool, &modelDescriptorLayout);
	}


  VtLogHandler::oStream("myproject", "main", "Starting");

	while(!window.shouldClose())
	{
		window.update();
	}
	
	VtLogHandler::oStream("myproject", "main", "Ending");
	return 0;
}
