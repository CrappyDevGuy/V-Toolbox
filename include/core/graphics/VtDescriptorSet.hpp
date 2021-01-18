//_Author : CrappyDevGuy, M01D18Y2021_//

#pragma once

#include "core/VtDevices.hpp"
#include "core/graphics/pool/VtDescriptorPool.hpp"
#include "core/graphics/layout/VtDescriptorLayout.hpp"

struct VtDescriptorSetCreateInfo
{
	std::string name{"NULL"};
	VtDevices*  pVtDevices{nullptr};
};

struct VtDescriptorSetBinding
{
  std::uint32_t    binding;
  VkDescriptorType descriptorType;
};

class VtDescriptorSet
{
  struct BindingData
  {
    std::uint32_t             binding;
    VkDescriptorType          descriptorType;

    std::vector<VkDescriptorImageInfo> imagesInfo;
    std::vector<VkDescriptorBufferInfo> buffersInfo;
  };
    
	public:
		VtDescriptorSet(VtDescriptorSetCreateInfo _createInfo);
		VtDescriptorSet(const VtDescriptorSet&) = delete;
		VtDescriptorSet() = default;
		~VtDescriptorSet();

		VtDescriptorSet& operator=(const VtDescriptorSet&) = delete;
		VtDescriptorSet& operator=(VtDescriptorSet&& other) noexcept;
		VtDescriptorSet(VtDescriptorSet&& other) noexcept;

    void addBinding(VtDescriptorSetBinding binding, VkDescriptorImageInfo imageInfo);
    void addBinding(VtDescriptorSetBinding binding, std::vector<VkDescriptorImageInfo> imagesInfo);
    void addBinding(VtDescriptorSetBinding binding, VkDescriptorBufferInfo bufferInfo);
    void addBinding(VtDescriptorSetBinding binding, std::vector<VkDescriptorBufferInfo> buffersInfo);

    void build(VtDescriptorPool* pVtDescriptorPool, VtDescriptorLayout* pVtDescriptorLayout);
    void update();

    inline auto& getInstance() noexcept { return m_descriptor; };

	private:
		std::string 		m_name{"NULL"};
		VtDevices*  		m_pVtDevices{nullptr};
		VkDescriptorSet m_descriptor{nullptr};

		std::vector<VtDescriptorSet::BindingData> m_descriptorWrites;
};
