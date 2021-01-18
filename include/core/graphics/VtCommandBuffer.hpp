//_Author : CrappyDevGuy, M01D18Y2021_//

#pragma once

#include "core/VtDevices.hpp"
#include "core/graphics/pool/VtCommandPool.hpp"

struct VtCommandBufferCreateInfo
{
	std::string name{"NULL"};

	VtDevices* pVtDevices{nullptr};
};

class VtCommandBuffer
{
	public:
		VtCommandBuffer(VtCommandBufferCreateInfo _createInfo);
		VtCommandBuffer(const VtCommandBuffer&) = delete;
		VtCommandBuffer() = default;
		~VtCommandBuffer();

		VtCommandBuffer& operator=(const VtCommandBuffer&) = delete;
		VtCommandBuffer& operator=(VtCommandBuffer&& other) noexcept;
		VtCommandBuffer(VtCommandBuffer&& other) noexcept;

		inline void bindCommandPool(VtCommandPool* pVtCommandPool)
		{ assert(pVtCommandPool != nullptr); m_pVtCommandPool = pVtCommandPool; };

		inline auto& getInstance() const noexcept { return m_commandBuffer; };

		void alloc(VtCommandPool* pVtCommandPool);
		void free();
		void begin();
		void end();
		void reset();

		void bindPipeline();

		void beginRenderPass(VkRenderPassBeginInfo info, VkSubpassContents contents);
		void endRenderPass();

		void bindVertexBuffers(std::vector<VkBuffer> buffers, std::vector<VkDeviceSize> offsets);
		void bindVertexBuffer(VkBuffer& rBuffer, VkDeviceSize offset);

		void bindIndexBuffer(VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType);

		void bindPipelineLayout(VkPipelineLayout* pPipelineLayout);
		void bindDescriptorSet(std::uint32_t firstSet, VkDescriptorSet& set);
		void bindDescriptorSets(std::uint32_t firstSet, std::vector<VkDescriptorSet> sets);

		void drawIndexed(std::uint32_t indicesCount, std::uint32_t instanceCount);
		void nextSubpass(VkSubpassContents contents);
		
	private:
		std::string 				m_name{"NULL"};
  	VtDevices*  				m_pVtDevices{nullptr};
		VtCommandPool*			m_pVtCommandPool{nullptr};
		VkCommandBuffer 	  m_commandBuffer{nullptr};

		VkPipelineLayout*	  m_tmpPipelineLayout{nullptr};
		VkPipelineBindPoint m_tmpPipelineBindPoint;
};

namespace VtCommandBufferUtil
{

}
