//_Author : CrappyDevGuy, M01D18Y2021_//

#pragma once

#include "core/VtDevices.hpp"
#include "core/VtSwapchain.hpp"

struct VtSubmitQueueCreateInfo
{
	std::string name{"NULL"};

	VtDevices*   pVtDevices{nullptr};
	VtSwapchain* pVtSwapchain{nullptr};
};

class VtSubmitQueue
{
	public:
		VtSubmitQueue(VtSubmitQueueCreateInfo _createInfo);
		VtSubmitQueue(const VtSubmitQueue&) = delete;
		VtSubmitQueue() = default;
		~VtSubmitQueue();

		VtSubmitQueue& operator=(const VtSubmitQueue&) = delete;
		VtSubmitQueue& operator=(VtSubmitQueue&& other) noexcept;
		VtSubmitQueue(VtSubmitQueue&& other) noexcept;

    bool submit(const std::vector<VkCommandBuffer>& t_commandBuffers);

	private:
		std::string  m_name{"NULL"};
		VtDevices*   m_pVtDevices{nullptr};
		VtSwapchain* m_pVtSwapchain{nullptr};

    std::vector<VkSemaphore> m_imagesAvailable;
    std::vector<VkSemaphore> m_rendersFinished;

    std::vector<VkFence>     m_inflightFences;
    std::vector<VkFence>     m_imagesinflight;
    std::size_t              m_currentFrame = 0;
    std::uint32_t            m_imageCount = 0;
};
