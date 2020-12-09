//_Author : CrappyDevGuy, M12D08Y2020_//

#include "core/VtInstance.hpp"

#include "core/util/VtUtil.hpp"

#include <GLFW/glfw3.h>
#include <cstring>

static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator);
static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback);
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
  VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
  VkDebugUtilsMessageTypeFlagsEXT messageType,
  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
  void* pUserData);

//_ Public Functions _// 
VtInstance& VtInstance::operator=(VtInstance&& other) noexcept
{
	std::swap(m_instance, other.m_instance);
	std::swap(m_callback, other.m_callback);
	std::swap(m_name, other.m_name);
	return *this;
}

VtInstance::VtInstance(VtInstance&& other) noexcept
:m_instance{std::exchange(other.m_instance, nullptr)}
,m_callback{std::exchange(other.m_callback, nullptr)}
,m_name{std::move(other.m_name)}
{}

VtInstance::VtInstance(VtInstanceCreateInfo _createInfo)
{
	m_name = std::move(_createInfo.appName);	

	VtUtil::checkVulkanResult(m_name+"_volk", volkInitialize());
	VtLogHandler::oStream("V-Toolbox", m_name+"::VtInstance", "Volk has been initialize");

	if(!glfwInit())
	{
		VtLogHandler::oStreamFatalError("V-Toolbox", m_name+"::VtInstance", "GLFW failed to initialize");
	}

	VkApplicationInfo appInfo = {};
	appInfo.sType                = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName     = m_name.c_str();
  appInfo.applicationVersion   = _createInfo.appVersion;
  appInfo.pEngineName          = "V-Toolbox";
  appInfo.engineVersion        = VK_MAKE_VERSION(1,0,0);
  appInfo.apiVersion           = VK_API_VERSION_1_0;

	auto extensions = getRequiredExtensions(true);
  VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo = {};
  populateDebugMessengerCreateInfo(debugMessengerCreateInfo);

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo        = &appInfo;
	createInfo.enabledExtensionCount   = static_cast<std::uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();
	createInfo.enabledLayerCount       = static_cast<std::uint32_t>(VtValidationLayers.size());
  createInfo.ppEnabledLayerNames     = VtValidationLayers.data();
  createInfo.pNext                   = (VkDebugUtilsMessengerCreateInfoEXT*) &debugMessengerCreateInfo;

  VtUtil::checkVulkanResult(m_name+"_VkInstance", vkCreateInstance(&createInfo, nullptr, &m_instance));

  volkLoadInstance(m_instance);

  VtUtil::checkVulkanResult(m_name+"_DebugMessenger", CreateDebugUtilsMessengerEXT(m_instance, &debugMessengerCreateInfo, nullptr, &m_callback));
  VtLogHandler::oStream("V-toolbox", m_name+"::VtInstance", "Success to create");
}

VtInstance::~VtInstance()
{
	if(m_callback != nullptr)
	{
		DestroyDebugUtilsMessengerEXT(m_instance, m_callback, nullptr);
		m_callback = nullptr;
		VtLogHandler::oStream("V-Toolbox", m_name+"::~VtInstance", "destroying Debug Messenger");
	}

	if(m_instance != nullptr)
	{
    vkDestroyInstance(m_instance, nullptr);
		m_instance = nullptr;
		VtLogHandler::oStream("V-Toolbox", m_name+"::~VtInstance", "destroying Instance");
	}
}

//_ Private Functions _//
std::vector<const char*> VtInstance::getRequiredExtensions(bool validationLayers)
{
  std::uint32_t glfwExtensionCount = 0;
  const char**  glfwExtensions;

  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

  if(validationLayers)
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

  return extensions;
}

bool VtInstance::checkValidationLayerSupport()
{
  std::uint32_t layerCount = 0;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for(const char* layerName : VtValidationLayers)
  {
    bool layerFound = false;
    for(const auto& layerProperties : availableLayers)
    {
      if(strcmp(layerName, layerProperties.layerName) == 0)
      {
        layerFound = true;
        break;
      }
    }
    if(!layerFound) return false;
  }

  return true;
}


//_ Static Functions _//
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
  VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
  VkDebugUtilsMessageTypeFlagsEXT messageType,
  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
  void* pUserData)
{

  if(messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
    VtLogHandler::oStreamWarning("V-Toolbox", "[instance::debugCallback]", std::string(pCallbackData->pMessage));
  }else if(messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
    VtLogHandler::oStreamFatalError("V-Toolbox", "[instance::debugCallback]", std::string(pCallbackData->pMessage));
  else
    VtLogHandler::oStreamDebug("V-Toolbox", "[instance::debugCallback]", std::string(pCallbackData->pMessage));

  return VK_FALSE;
}

static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback)
{
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr)
     return func(instance, pCreateInfo, pAllocator, pCallback);

  return VK_ERROR_EXTENSION_NOT_PRESENT;
}

static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator)
{
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr)
      func(instance, callback, pAllocator);
}

static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
  createInfo.sType 	         = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
			                       | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
	                           | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  createInfo.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                  			     | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                  		       | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo.pfnUserCallback = debugCallback;
  createInfo.pUserData       = nullptr;
}
