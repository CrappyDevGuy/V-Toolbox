//_Author : CrappyDevGuy, M12D29Y2020_//

#include "core/VtWindow.hpp"

#include "core/io/VtLogHandler.hpp"
#include "core/util/VtUtil.hpp"
//_ Public Functions _//
namespace VtInternWindow
{
  double MouseX = 0, MouseY = 0;
}

/* Public Functions */
static void framebufferSizeCallback(GLFWwindow* p_window, int width, int height)
{
  auto app = reinterpret_cast<VtWindow*>(glfwGetWindowUserPointer(p_window));
  app->setResize(true);
}

static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
  VtInternWindow::MouseX = xpos;
  VtInternWindow::MouseY = ypos;
}

static void error_callback(int error, const char* description)
{
	VtLogHandler::oStreamError("V-Toolbox", "VtWindow::error_callback", std::to_string(error) + " / " + std::string(description));
}

VtWindow& VtWindow::operator=(VtWindow&& other) noexcept
{
	std::swap(m_data,    other.m_data);
	std::swap(m_window,  other.m_window);
	std::swap(m_surface, other.m_surface);
	return *this;
}

VtWindow::VtWindow(VtWindow&& other) noexcept
:m_data{std::move(other.m_data)}
,m_window{std::exchange(other.m_window, nullptr)}
,m_surface{std::exchange(other.m_surface, nullptr)}
{}

VtWindow::VtWindow(VtWindowCreateInfo _createInfo)
{
  assert(_createInfo.pVtInstance != nullptr);

  m_data.name   = std::move(_createInfo.name);
	m_data.extent	= std::move(_createInfo.extent);
	m_pVtInstance = _createInfo.pVtInstance;
	

	for(auto i = 0; i < _createInfo.windowhints.size(); i++)
	{
		auto hint  = _createInfo.windowhints[i].x;
		auto value = _createInfo.windowhints[i].y;
		
		glfwWindowHint(hint, value);
	}

	m_window = glfwCreateWindow(m_data.extent.x, m_data.extent.y, m_data.name.c_str(), nullptr, nullptr);
	
	glfwSetWindowUserPointer(m_window, this);
	glfwSetErrorCallback(error_callback);
	glfwSetCursorPosCallback(m_window, mouse_callback);
	
	glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);
	
	VtUtil::checkVulkanResult(m_data.name+"::VtWindow::glfwCreateWindowSurface", glfwCreateWindowSurface(m_pVtInstance->getVkInstance(), m_window, nullptr, &m_surface));
	VtLogHandler::oStream("V-Toolbox", m_data.name+"::VtWindow", "Success to create");
}

VtWindow::~VtWindow()
{
	if(m_window != nullptr)
  {
    glfwDestroyWindow(m_window);
    m_window = nullptr;
    VtLogHandler::oStream("V-Toolbox", m_data.name+"::~VtWindow", "Success to destroy GLFWwindow");
    glfwTerminate();
  }
  
  if(m_surface != nullptr)
  {
    vkDestroySurfaceKHR(m_pVtInstance->getVkInstance(), m_surface, nullptr);
    m_surface = VK_NULL_HANDLE;
    VtLogHandler::oStreamDebug("V-Toolbox", m_data.name+"::~VtWindow", "Success to destroy VkSurface");
  }
}

void VtWindow::update()
{
  assert(m_window != nullptr);

  m_data.mousePosition = {VtInternWindow::MouseX, VtInternWindow::MouseY};

  int width = 0, height = 0;
  glfwGetFramebufferSize(m_window, &width, &height);
  while(width == 0 && height == 0)
  {
    glfwGetFramebufferSize(m_window, &width, &height);
    glfwWaitEvents();
  }
  glfwPollEvents();
}
