//_Author : CrappyDevGuy, M01D18Y2021_//

#pragma once

#include "core/VtWindow.hpp"

struct VtKeyboardCreateInfo
{
	std::string name{"NULL"};
	VtWindow* pVtWindow{nullptr};
};

class VtKeyboard
{
	public:
		VtKeyboard(VtKeyboardCreateInfo _createInfo);
		VtKeyboard(const VtKeyboard&) = delete;
		VtKeyboard() = default;
		~VtKeyboard();

		VtKeyboard& operator=(const VtKeyboard&) = delete;
		VtKeyboard& operator=(VtKeyboard&& other) noexcept;
		VtKeyboard(VtKeyboard&& other) noexcept;

    bool isPressed(int KEY);
    bool isPressedOnce(int KEY);

	private:
		std::string m_name{"NULL"};
		VtWindow*   m_pVtWindow{nullptr};

		std::vector<bool> m_keysStats;
};
