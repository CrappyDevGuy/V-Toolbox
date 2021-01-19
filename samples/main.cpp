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
#include "core/graphics/VtSubmitQueue.hpp"
#include "core/VtImporter.hpp"
#include "core/resources/VtMesh.hpp"

#include "core/io/VtKeyboard.hpp"
#include "core/io/VtMouse.hpp"
#include "core/view/VtCamera.hpp"
#include "core/entities/VtObject.hpp"

#include <iostream>
#include <iomanip>

constexpr static glm::ivec2 appExtent{1280, 720};

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
//	swapchainCreateInfo.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;

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
	descriptorPoolCreateInfo.setsCount  = 1000;
	descriptorPoolCreateInfo.pVtDevices = &devices;
	descriptorPoolCreateInfo.poolsData  = {
		{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000}
	};

	VtDescriptorPool descriptorPool{descriptorPoolCreateInfo};

	VtShaderInput shaderInput{};
	shaderInput.addInput(0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0);
	shaderInput.addBinding(0, VK_VERTEX_INPUT_RATE_VERTEX, 3*sizeof(float));
	shaderInput.addInput(1, 1, VK_FORMAT_R32G32B32_SFLOAT, 0);
	shaderInput.addBinding(1, VK_VERTEX_INPUT_RATE_VERTEX, 3*sizeof(float));

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
	mainPipeline.setViewport(appExtent);
	mainPipeline.setCullFace(VK_CULL_MODE_NONE);
	mainPipeline.setSamples(devices.getSamplesCount());
	mainPipeline.setBindPoint(VK_PIPELINE_BIND_POINT_GRAPHICS);
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

	VtDescriptorSet mainDescriptorSet{{"MainDescriptor", &devices}};
	mainDescriptorSet.addBinding({0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER}, mainViewUB.getDescriptorSetInfo());
	mainDescriptorSet.build(&descriptorPool, &mainDescriptorLayout);
	mainDescriptorSet.update(); 
	
	VtSubmitQueueCreateInfo submitQueueCreateInfo{};
	submitQueueCreateInfo.name 				 = "MainSubmitQueue";
	submitQueueCreateInfo.pVtDevices 	 = &devices;
	submitQueueCreateInfo.pVtSwapchain = &swapchain;
	VtSubmitQueue submitQueue{submitQueueCreateInfo};

	VtImporter::LoadInfo modelLoadInfo{};
	modelLoadInfo.name 				= "Model0";
	modelLoadInfo.path 				= "../samples/res/models/scene.fbx";
	modelLoadInfo.assimpflags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs;

	VtImporter::FileData modelFileData{};
	VtImporter::load(modelLoadInfo, modelFileData);

	auto meshesCount = modelFileData.meshes.size();
	std::vector<VtBuffer> modelMaterials(meshesCount);
	std::vector<VtMesh> modelMeshes(meshesCount);
	std::vector<VtDescriptorSet> modelMeshesDescriptor(meshesCount);

	for(auto i =0u; i < meshesCount; i++)
	{
		Material materialData{};
		materialData.ambient = glm::vec4{0.2f};
		materialData.diffuse = glm::vec4{modelFileData.materials[i].diffuse, 1.0f};
		modelMaterials[i] = {{"modelMaterials["+std::to_string(i)+"]", &devices, sizeof(Material), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT}};
		modelMaterials[i].mapMemory(&materialData);

		modelMeshes[i] = {{"Mesh[" + std::to_string(i) + "]", &devices}};
		modelMeshes[i].createIndicesBuffer(modelFileData.meshes[i].indices.size()*sizeof(std::uint32_t));
		modelMeshes[i].createVerticesBuffer("Vertices", modelFileData.meshes[i].vertices.size()*sizeof(float), 0);
		modelMeshes[i].createVerticesBuffer("Normals", modelFileData.meshes[i].normals.size()*sizeof(float), 0);

		modelMeshes[i].setIndices(modelFileData.meshes[i].indices, commandBuffers[0]);
		modelMeshes[i].setVertices(0, modelFileData.meshes[i].vertices, commandBuffers[0]);
		modelMeshes[i].setVertices(1, modelFileData.meshes[i].normals, commandBuffers[0]);

		modelMeshesDescriptor[i] = {{"ModelDescriptorSet["+std::to_string(i)+"]", &devices}};
		modelMeshesDescriptor[i].addBinding({0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER}, modelTransformUB.getDescriptorSetInfo());
	  modelMeshesDescriptor[i].addBinding({1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER}, modelMaterials[i].getDescriptorSetInfo());
		modelMeshesDescriptor[i].build(&descriptorPool, &modelDescriptorLayout);
		modelMeshesDescriptor[i].update();
	}

	commandBuffers[0].reset();

  std::vector<VkClearValue> clearValues(3);
  clearValues[0].color 			  = {0.0f, 0.0f, 0.0f, 1.0f};
  clearValues[1].depthStencil = {1.0f, 0};
  clearValues[2].color				= {0.0f, 0.0f, 0.0f, 1.0f};

	for(auto i = 0u; i < swapchain.getImageCount(); i++)
	{
		commandBuffers[i].begin();
		commandBuffers[i].bindPipeline(&mainPipeline);

	  VkRect2D area = {};
	  area.offset        = {0,0};
	  area.extent.width  = static_cast<std::uint32_t>(appExtent.x);
	  area.extent.height = static_cast<std::uint32_t>(appExtent.y);

	  VkRenderPassBeginInfo renderpassInfo = {};
	  renderpassInfo.sType       = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	  renderpassInfo.renderPass  = mainRenderpass.getInstance(); 
	  renderpassInfo.framebuffer = framebuffers[i].getInstance();
	  renderpassInfo.renderArea  = area;

	  renderpassInfo.clearValueCount = static_cast<std::uint32_t>(clearValues.size());
	  renderpassInfo.pClearValues    = clearValues.data();
	  
		commandBuffers[i].beginRenderPass(renderpassInfo, VK_SUBPASS_CONTENTS_INLINE);
			commandBuffers[i].bindPipelineLayout(&mainPipelinelayout.getInstance());
			commandBuffers[i].bindDescriptorSet(0, mainDescriptorSet.getInstance());

			for(auto j = 0; j < meshesCount; j++)
			{
				commandBuffers[i].bindDescriptorSet(1, modelMeshesDescriptor[j].getInstance());

				auto buffers = {
												modelMeshes[j].getVerticesBuffer(0).getBufferInstance(),
												modelMeshes[j].getVerticesBuffer(1).getBufferInstance()
											 };

				commandBuffers[i].bindVertexBuffers(buffers, {0, 0});
				commandBuffers[i].bindIndexBuffer(modelMeshes[j].getIndicesBuffer().getBufferInstance(), {0}, VK_INDEX_TYPE_UINT32);
				commandBuffers[i].drawIndexed(modelMeshes[j].getIndicesCount(), 1);
			}
		
		commandBuffers[i].endRenderPass();
		commandBuffers[i].end();
	}
	
	std::vector<VkCommandBuffer> cbs(3);
	cbs[0] = commandBuffers[0].getInstance();
	cbs[1] = commandBuffers[1].getInstance();
	cbs[2] = commandBuffers[2].getInstance();

	VtKeyboard keyboard{{"MainKeyboard", &window}};
	VtMouse mouse{{"MainMouse", &window}};

	VtCameraCreateInfo cameraCreateInfo{};
	cameraCreateInfo.position = glm::vec3{-2.0f, 2.0f, -2.0f};
	cameraCreateInfo.rotation = glm::vec3{-35.0f, -45.0f, 0.0f};
	cameraCreateInfo.zValue   = glm::vec2{0.001f, 500.0f};
	
	VtCamera camera{cameraCreateInfo};
	camera.createPerspectiveMatrix(90.0f, appExtent.x, appExtent.y);
	camera.createViewMatrix();

	ViewMatrices matrices{};
	matrices.viewMatrix = camera.getCopyViewMatrix();
	matrices.projMatrix = camera.getCopyProjectionMatrix();
	mainViewUB.mapMemory(&matrices);

	VtObject model{glm::vec3{0.0f}, glm::vec3{-90.0f, 0.0f, 0.0f}};
	model.createTransformationMatrix();

	ModelMatrices modelMatrices{};
	modelMatrices.transformMatrix = model.getCopyTransformation();
	modelTransformUB.mapMemory(&modelMatrices);

  VtLogHandler::oStream("myproject", "main", "Starting");

	float cameraSpeed = 0.01f;
	glm::vec3 nextPos;
	bool isFocused = false;
	glm::vec3 direction;


  double previousTime = glfwGetTime();
  int frameCount = 0, tickCount = 0;

  double tickCap = 1.0f / 60.0f;
  float lastTick = 0.0f;

  double frameCap = 1.0f / 75.0f;
  float lastFrame = 0.0f;
	
	while(!window.shouldClose())
	{

		if(glfwGetTime() - lastTick >= tickCap)
		{
			if(keyboard.isPressed(GLFW_KEY_W))
				direction.z += cameraSpeed;
			else if(keyboard.isPressed(GLFW_KEY_S))
				direction.z -= cameraSpeed;

			if(keyboard.isPressed(GLFW_KEY_A))
				direction.x += cameraSpeed;
			else if(keyboard.isPressed(GLFW_KEY_D))
				direction.x -= cameraSpeed;

			if(keyboard.isPressed(GLFW_KEY_SPACE))
				direction.y += cameraSpeed;
			else if(keyboard.isPressed(GLFW_KEY_Q))
				direction.y -= cameraSpeed;

			if(keyboard.isPressedOnce(GLFW_KEY_LEFT_CONTROL))
			{
				isFocused = !isFocused;

				if(isFocused)
					mouse.setCursorMode(GLFW_CURSOR_DISABLED);
				else
					mouse.setCursorMode(GLFW_CURSOR_NORMAL);
			}

			if(isFocused)
			{
	      glm::vec2 rotation = mouse.getMousePosition();
	      camera.VtEntity::setRotation(glm::vec3{-rotation.y / 10, rotation.x / 10, 0});
			}

			float value = glm::radians(camera.VtEntity::getRefRotation().y);
	    nextPos.x += direction.x * glm::cos(value) - direction.z * glm::sin(value);
	    nextPos.y += direction.y;
	    nextPos.z += direction.z * glm::cos(value) + direction.x * glm::sin(value);

			camera.VtEntity::incPosition(nextPos);
			camera.createViewMatrix();
			matrices.viewMatrix = camera.getCopyViewMatrix();
			mainViewUB.mapMemory(&matrices);

	    direction = glm::vec3(0.0f);
	    nextPos *= glm::vec3(0.8f);
	    
      tickCount++;
      lastTick += tickCap;
    }

		window.update();
		if(glfwGetTime() - lastFrame >= frameCap)
    {
			submitQueue.submit(cbs);
      frameCount++;
      lastFrame += frameCap;
    }

    double currentTime = glfwGetTime();

    if(currentTime - previousTime >= 1.0)
    {
      double time = currentTime-previousTime;

			auto camPos = camera.VtEntity::getRefPosition();
			auto camRot = camera.VtEntity::getRefRotation();
			std::string pos = "{"+std::to_string(camPos.x)+", "+std::to_string(camPos.y)+", "+std::to_string(camPos.z)+"}";
			std::string rot = "{"+std::to_string(camRot.x)+", "+std::to_string(camRot.y)+", "+std::to_string(camRot.z)+"}";
		
      std::string info = "FPS : " + std::to_string(frameCount) + " | TPS : " + std::to_string(tickCount) + " | " + pos + " | " + rot;
      VtLogHandler::oStream("myproject", "main", info);
      frameCount = 0;
      tickCount = 0;
      previousTime = currentTime;
    }
	}

	devices.waitDevice();
	
	VtLogHandler::oStream("myproject", "main", "Ending");
	return 0;
}
