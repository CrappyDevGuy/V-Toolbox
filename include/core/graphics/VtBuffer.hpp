//_Author : CrappyDevGuy, M12D29Y2020_//

#include "core/VtDevices.hpp"

#include "core/util/VtUtil.hpp"
#include "core/graphics/VtImage.hpp"
#include <cstring>

struct VtBufferCreateInfo
{
	std::string name{"NULL"};
	VtDevices*  pVtDevices{nullptr};
	
  VkDeviceSize          size;
  VkBufferUsageFlags    usage;
  VkMemoryPropertyFlags properties{VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};
};

class VtBuffer
{
	public:
	  VtBuffer(VtBufferCreateInfo _createInfo);
    VtBuffer(const VtBuffer&) = delete;
    VtBuffer() = default;
    ~VtBuffer();

    VtBuffer& operator=(const VtBuffer&) = delete;
    VtBuffer& operator=(VtBuffer&& other) noexcept;
    VtBuffer(VtBuffer&& other) noexcept;

		template<typename T>
    void mapVecMemory(std::vector<T> t_data)
    {
      void* data;
      VtUtil::checkVulkanResult(m_name+"::Mapping", vkMapMemory(m_pVtDevices->getLogicalDevice(), m_memory, 0, m_size, 0, &data));
      memcpy(data, t_data.data(), (std::size_t)m_size);
      vkUnmapMemory(m_pVtDevices->getLogicalDevice(), m_memory);
    }

    template<typename T>
    void mapMemory(T t_data)
    {
      void* data;
      VtUtil::checkVulkanResult(m_name+"::Mapping", vkMapMemory(m_pVtDevices->getLogicalDevice(), m_memory, 0, m_size, 0, &data));
      memcpy(data, t_data, (std::size_t)m_size);
      vkUnmapMemory(m_pVtDevices->getLogicalDevice(), m_memory);
    }

    void createView(VkFormat format, VkDeviceSize offset, VkDeviceSize range);
    void copyToImage(VkCommandBuffer& commandBuffer, VtImage& rImage);

    inline auto getDescriptorSetInfo() noexcept { return VkDescriptorBufferInfo{m_buffer, 0, m_size}; };

	private:
    std::string   m_name;
    VkDeviceSize  m_size{0};
    VtDevices*    m_pVtDevices{nullptr};

    VkBuffer       m_buffer{nullptr};
    VkDeviceMemory m_memory{nullptr};
    VkBufferView   m_view{nullptr};
	
};
