//_Author : CrappyDevGuy, M01D18Y2021_//

#include "core/io/VtKeyboard.hpp"
#include "core/io/VtLogHandler.hpp"

VtKeyboard& VtKeyboard::operator=(VtKeyboard&& other) noexcept
{
	std::swap(other.m_name, m_name);
	std::swap(other.m_pVtWindow, m_pVtWindow);
	std::swap(other.m_keysStats, m_keysStats);
	return *this;
}

VtKeyboard::VtKeyboard(VtKeyboard&& other) noexcept
:m_name{std::move(other.m_name)}
,m_pVtWindow{std::exchange(other.m_pVtWindow, nullptr)}
,m_keysStats{std::move(other.m_keysStats)}
{}

VtKeyboard::VtKeyboard(VtKeyboardCreateInfo _createInfo)
{
	assert(_createInfo.pVtWindow != nullptr);

	m_name      = std::move(_createInfo.name);
	m_pVtWindow = _createInfo.pVtWindow;

	m_keysStats = std::vector<bool>(1000, false);
	VtLogHandler::oStream("V-Toolbox", m_name+"::VtKeyboard", "Success to create");
}

VtKeyboard::~VtKeyboard()
{
	if(!m_keysStats.empty())
	{
		m_keysStats.clear();
		VtLogHandler::oStream("V-Toolbox", m_name+"::~VtKeyboard", "Success to destroy");
	}
}

bool VtKeyboard::isPressedOnce(int KEY)
{
  assert(m_pVtWindow != nullptr);

  bool isPressed = glfwGetKey(m_pVtWindow->getWindowInstance(), KEY);
  if(m_keysStats[KEY] == false && isPressed == true)
  {
    m_keysStats[KEY] = true;
    return isPressed;
  } else if(m_keysStats[KEY] == true && isPressed == false)
    m_keysStats[KEY] = false;

  return false;
}

bool VtKeyboard::isPressed(int KEY)
{
  assert(m_pVtWindow != nullptr);
  return glfwGetKey(m_pVtWindow->getWindowInstance(), KEY);
}
