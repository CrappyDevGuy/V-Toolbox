//_Author : CrappyDevGuy, M01D18Y2021_//

#include "core/resources/VtMesh.hpp"

#include "core/io/VtLogHandler.hpp"

VtMesh& VtMesh::operator=(VtMesh&& other) noexcept
{
	std::swap(other.m_name, m_name);
	std::swap(other.m_pVtDevices, m_pVtDevices);
	return *this;
}

VtMesh::VtMesh(VtMesh&& other) noexcept
:m_name{std::move(other.m_name)}
,m_pVtDevices{std::exchange(other.m_pVtDevices, nullptr)}
{}

VtMesh::VtMesh(VtMeshCreateInfo _createInfo)
{
	assert(_createInfo.pVtDevices != nullptr);

	m_name       = std::move(_createInfo.name);
	m_pVtDevices = _createInfo.pVtDevices;

	VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::VtMesh", "Success to create");
}

VtMesh::~VtMesh()
{
	if(m_pVtDevices != nullptr)
	{
		VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::~VtMesh", "Success to destroy");		
	}
}

void VtMesh::createIndicesBuffer(std::uint32_t size)
{
  assert(size > 0);

  VtBufferCreateInfo indicesBufferInfo = {};
  indicesBufferInfo.name       = m_name+"_IndicesBuffer";
  indicesBufferInfo.size       = size;
  indicesBufferInfo.usage      = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
  indicesBufferInfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
  indicesBufferInfo.pVtDevices = m_pVtDevices;
  m_bufferIndices = {indicesBufferInfo};
}

void VtMesh::createVerticesBuffer(std::string name, std::uint32_t size, VkDeviceSize offset)
{
  assert(size > 0);

  VtBufferCreateInfo createInfo = {};
  createInfo.name       = m_name+"_"+name;
  createInfo.size       = size;
  createInfo.usage      = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
  createInfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
  createInfo.pVtDevices = m_pVtDevices;
  m_bufferVertices.emplace_back(createInfo);
  m_bufferOffsets.push_back(std::move(offset));
}

void VtMesh::setIndices(std::vector<std::uint32_t> t_indices, VtCommandBuffer& copyCommandBuffer)
{
  std::uint32_t size = sizeof(std::uint32_t)*t_indices.size();
  VtBufferCreateInfo stagingCreateInfo = {};
  stagingCreateInfo.name       = m_name+"_IndicesStagingBuffer";
  stagingCreateInfo.size       = size;
  stagingCreateInfo.usage      = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
  stagingCreateInfo.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
  stagingCreateInfo.pVtDevices = m_pVtDevices;

  VtBuffer stagingBuffer{stagingCreateInfo};
  stagingBuffer.mapVecMemory(t_indices);

  copyCommandBuffer.copyBuffer(stagingBuffer.getBufferInstance(), m_bufferIndices.getBufferInstance(), size);
  m_indicesCount = t_indices.size();
}

std::vector<VkBuffer> VtMesh::getVerticesBufferInstances()
{
  std::vector<VkBuffer> buffers(m_bufferVertices.size());
  for(auto i = 0u; i < m_bufferVertices.size(); i++)
    buffers[i] = m_bufferVertices[i].getBufferInstance();
  return buffers;
}

std::vector<VkDeviceSize> VtMesh::getVerticesBufferOffsets()
{
  return m_bufferOffsets;
}
