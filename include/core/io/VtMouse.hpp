//_Author : CrappyDevGuy, M01D18Y2021_//

#pragma once 

#include "core/VtWindow.hpp"

struct VtMouseCreateInfo
{
	std::string name{"NULL"};
	VtWindow* pVtWindow{nullptr};
};

class VtMouse
{
	public:
		VtMouse(VtMouseCreateInfo _createInfo);
		VtMouse(const VtMouse&) = delete;
		VtMouse() = default;
		~VtMouse();

		VtMouse& operator=(const VtMouse&) = delete;
		VtMouse& operator=(VtMouse&& other) noexcept;
		VtMouse(VtMouse&& other) noexcept;

    bool isPressed(int KEY);
    bool isPressedOnce(int KEY);

    void setCursorMode(int MODE);

    inline auto getMousePosition() { return m_pVtWindow->getMousePosition(); };

	private:
		std::string m_name{"NULL"};
		VtWindow*   m_pVtWindow{nullptr};

		std::vector<bool> m_keysStats;
};
