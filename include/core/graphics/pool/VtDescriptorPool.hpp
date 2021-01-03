//_Author : CrappyDevGuy, M01D03Y2021_//

#pragma once

#include "core/VtDevices.hpp"

struct VtDescriptorPoolCreateInfo
{
	std::string   name{"NULL"};
  std::uint32_t setsCount{0};
	VtDevices*    pVtDevices{nullptr};
	
	std::vector<VkDescriptorPoolSize> poolsData;
};

class VtDescriptorPool
{
	public:
		VtDescriptorPool(VtDescriptorPoolCreateInfo _createInfo);
		VtDescriptorPool(const VtDescriptorPool&) = delete;
		VtDescriptorPool() = default;
		~VtDescriptorPool();

		VtDescriptorPool& operator=(const VtDescriptorPool&) = delete;
		VtDescriptorPool& operator=(VtDescriptorPool&& other) noexcept;
		VtDescriptorPool(VtDescriptorPool&& other) noexcept;

		inline auto& 						getInstance()  const noexcept { return m_descriptorpool; };
		inline std::string_view getName()			 const noexcept { return m_name; 					 };
		inline std::uint32_t		getSetsCount() const noexcept { return m_setsCount;      };

	private:
    std::string      m_name{"NULL"};
    std::uint32_t    m_setsCount{0};
    VtDevices*       m_pVtDevices{nullptr};
    VkDescriptorPool m_descriptorpool{nullptr};
};
