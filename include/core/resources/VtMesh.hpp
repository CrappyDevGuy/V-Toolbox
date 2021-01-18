//_Author : CrappyDevGuy, M01D18Y2021_//

#pragma once

#include "core/VtDevices.hpp"
#include "core/graphics/VtBuffer.hpp"
#include "core/graphics/VtCommandBuffer.hpp"

struct VtMeshCreateInfo
{
	std::string name{"NULL"};
	VtDevices*  pVtDevices{nullptr};
};

class VtMesh
{
	public:
		VtMesh(VtMeshCreateInfo _createInfo);
    VtMesh(const VtMesh&) = delete;
    VtMesh() = default;
    ~VtMesh();

    VtMesh& operator=(const VtMesh&) = delete;
    VtMesh& operator=(VtMesh&& other) noexcept;
    VtMesh(VtMesh&& other) noexcept;

		void createIndicesBuffer(std::uint32_t size);
    void createVerticesBuffer(std::string name, std::uint32_t size, VkDeviceSize offset);

    void setIndices(std::vector<std::uint32_t> t_indices, VtCommandBuffer& copyCommandBuffer);
    template<typename T>
    void setVertices(std::uint32_t bufferIndex, std::vector<T> t_vertices, VtCommandBuffer& copyCommandBuffer)
    {
      std::uint32_t size = t_vertices.size()*sizeof(T);
      VtBufferCreateInfo stagingCreateInfo = {};
      stagingCreateInfo.name       = m_bufferVertices[bufferIndex].getName()+"_StagingBuffer";
      stagingCreateInfo.size       = size;
      stagingCreateInfo.usage      = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
      stagingCreateInfo.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
      stagingCreateInfo.pVtDevices = m_pVtDevices;

      VtBuffer stagingBuffer{stagingCreateInfo};
      stagingBuffer.mapVecMemory(t_vertices);
      copyCommandBuffer.copyBuffer(stagingBuffer.getBufferInstance(), m_bufferVertices[bufferIndex].getBufferInstance(), size);
    }
    
    std::vector<VkBuffer>     getVerticesBufferInstances();
    std::vector<VkDeviceSize> getVerticesBufferOffsets();
    
    inline std::uint32_t getIndicesCount() 				const noexcept { return m_indicesCount; 					};
    inline std::uint32_t getVerticesBufferCount() const noexcept { return m_bufferVertices.size(); };

    inline auto& getVerticesBuffer(std::uint32_t index) noexcept { return m_bufferVertices[index]; };
    inline auto& getIndicesBuffer()											noexcept { return m_bufferIndices; 				 };

	private:
		std::string m_name{"NULL"};
		VtDevices*  m_pVtDevices{nullptr};
		
    std::uint32_t 						 m_indicesCount{0};
    VtBuffer                   m_bufferIndices;
    std::vector<VtBuffer>      m_bufferVertices;
    std::vector<VkDeviceSize>  m_bufferOffsets;
};
