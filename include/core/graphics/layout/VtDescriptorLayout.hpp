//_Author : CrappyDevGuy, M01D18Y2021_//

#pragma once

#include "core/VtDevices.hpp"

struct VtDescriptorLayoutBindingInfo
{
  std::uint32_t      binding;
  VkDescriptorType   dataType;
  std::uint32_t      dataCount;
  VkShaderStageFlags shaderStage;
};

struct VtDescriptorLayoutCreateInfo
{
	std::string name{"NULL"};
	VtDevices*  pVtDevices{nullptr};
};

class VtDescriptorLayout
{
	public:
		VtDescriptorLayout(VtDescriptorLayoutCreateInfo _createInfo);
	  VtDescriptorLayout(const VtDescriptorLayout&) = delete;
    VtDescriptorLayout()=default;
    ~VtDescriptorLayout();

    VtDescriptorLayout& operator=(const VtDescriptorLayout&) = delete;
    VtDescriptorLayout& operator=(VtDescriptorLayout&& other) noexcept;
    VtDescriptorLayout(VtDescriptorLayout&& other) noexcept;

  	void addBinding(VtDescriptorLayoutBindingInfo info);
  	void addBindings(std::vector<VtDescriptorLayoutBindingInfo> info);

  	void build();

    inline auto& getInstance() const noexcept { return m_descriptorlayout; };

	private:
		std::string 				  m_name{"NULL"};
		VtDevices*  				  m_pVtDevices{nullptr};
    VkDescriptorSetLayout m_descriptorlayout{nullptr};

    std::vector<VkDescriptorSetLayoutBinding> m_bindings;
};
