//_Author : CrappyDevGuy, M01D18Y2021_//

#pragma once

#include "core/VtDevices.hpp"
#include "core/graphics/layout/VtDescriptorLayout.hpp"

struct VtPipelineLayoutCreateInfo
{
	std::string name{"NULL"};
	VtDevices*  pVtDevices{nullptr};
};

class VtPipelineLayout
{
	public:
		VtPipelineLayout(VtPipelineLayoutCreateInfo _createInfo);
	  VtPipelineLayout(const VtPipelineLayout&) = delete;
    VtPipelineLayout()=default;
    ~VtPipelineLayout();

    VtPipelineLayout& operator=(const VtPipelineLayout&) = delete;
    VtPipelineLayout& operator=(VtPipelineLayout&& other) noexcept;
    VtPipelineLayout(VtPipelineLayout&& other) noexcept;

    void build();

    void addDescriptorLayout(VtDescriptorLayout* pDescriptorLayout);
    
    inline auto& getInstance() noexcept { return m_pipelineLayout; };

	private:
		std::string      m_name{"NULL"};
		VtDevices*       m_pVtDevices{nullptr};
		VkPipelineLayout m_pipelineLayout{nullptr};

		std::vector<VkDescriptorSetLayout> m_layouts;
};
