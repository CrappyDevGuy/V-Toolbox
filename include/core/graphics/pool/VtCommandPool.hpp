//_Author : CrappyDevGuy, M01D03Y2021_//

#pragma once

#include "core/VtDevices.hpp"

struct VtCommandPoolCreateInfo
{
	std::string name{"NULL"};

	VkCommandPoolCreateFlags flags;
	std::uint32_t queueFamilyIndex{0};
	
	VtDevices* pVtDevices{nullptr};
};

class VtCommandPool
{
	public:
		VtCommandPool(VtCommandPoolCreateInfo _createInfo);
		VtCommandPool(const VtCommandPool&) = delete;
		VtCommandPool() = default;
		~VtCommandPool();

		VtCommandPool& operator=(const VtCommandPool&) = delete;
		VtCommandPool& operator=(VtCommandPool&& other) noexcept;
		VtCommandPool(VtCommandPool&& other) noexcept;

		inline auto& 						getInstance() const noexcept { return m_commandpool; };
		inline std::string_view getName()			const noexcept { return m_name; 			 };

	private:
    std::string m_name{"NULL"};
    VtDevices*  m_pVtDevices{nullptr};

    VkCommandPool m_commandpool{nullptr};

};
