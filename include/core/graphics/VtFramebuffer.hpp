//_Author : CrappyDevGuy, M01D18Y2021_//

#pragma once

#include "core/VtDevices.hpp"
#include "core/graphics/VtRenderpass.hpp"

struct VtFramebufferCreateInfo
{
	std::string   name{"NULL"};
	VtDevices*    pVtDevices{nullptr};
	VtRenderpass* pVtRenderpass{nullptr};

	glm::ivec2 extent{800,600};

	std::vector<VkImageView> imagesView;
};

class VtFramebuffer
{
	public:
		VtFramebuffer(VtFramebufferCreateInfo _createInfo);
		VtFramebuffer(const VtFramebuffer&) = delete;
		VtFramebuffer() = default;
		~VtFramebuffer();

		VtFramebuffer& operator=(const VtFramebuffer&) = delete;
		VtFramebuffer& operator=(VtFramebuffer&& other) noexcept;
		VtFramebuffer(VtFramebuffer&& other) noexcept;

		inline auto& getInstance() const noexcept { return m_framebuffer; };

	private:
		std::string   m_name{"NULL"};
		VtDevices*    m_pVtDevices{nullptr};
		VkFramebuffer m_framebuffer{nullptr};
		glm::ivec2    m_extent{800,600};
};
