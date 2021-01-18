//_Author : CrappyDevGuy, M01D18Y2021_//

#include "core/io/VtMouse.hpp"
#include "core/io/VtLogHandler.hpp"

VtMouse& VtMouse::operator=(VtMouse&& other) noexcept
{
	std::swap(other.m_name, m_name);
	std::swap(other.m_pVtWindow, m_pVtWindow);
	std::swap(other.m_keysStats, m_keysStats);
	return *this;
}

VtMouse::VtMouse(VtMouse&& other) noexcept
:m_name{std::move(other.m_name)}
,m_pVtWindow{std::exchange(other.m_pVtWindow, nullptr)}
,m_keysStats{std::move(other.m_keysStats)}
{}

VtMouse::VtMouse(VtMouseCreateInfo _createInfo)
{
	assert(_createInfo.pVtWindow != nullptr);

	m_name      = std::move(_createInfo.name);
	m_pVtWindow = _createInfo.pVtWindow;

	m_keysStats = std::vector<bool>(8, false);
	VtLogHandler::oStream("V-Toolbox", m_name+"::VtMouse", "Success to create");
}

VtMouse::~VtMouse()
{
	if(!m_keysStats.empty())
	{
		m_keysStats.clear();
		VtLogHandler::oStream("V-Toolbox", m_name+"::~VtMouse", "Success to destroy");
	}
}

bool VtMouse::isPressedOnce(int KEY)
{
  assert(m_pVtWindow != nullptr);

  bool isPressed = glfwGetMouseButton(m_pVtWindow->getWindowInstance(), KEY);
  if(m_keysStats[KEY] == false && isPressed == true)
  {
    m_keysStats[KEY] = true;
    return isPressed;
  } else if(m_keysStats[KEY] == true && isPressed == false)
    m_keysStats[KEY] = false;

  return false;
}

bool VtMouse::isPressed(int KEY)
{
  assert(m_pVtWindow != nullptr);
  return glfwGetMouseButton(m_pVtWindow->getWindowInstance(), KEY);
}

void VtMouse::setCursorMode(int MODE)
{
  assert(m_pVtWindow != nullptr);
  glfwSetInputMode(m_pVtWindow->getWindowInstance(), GLFW_CURSOR, MODE);
}
