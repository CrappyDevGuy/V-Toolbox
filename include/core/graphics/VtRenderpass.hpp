//_Author : CrappyDevGuy, M01D18Y2021_//

#pragma once

#include "core/VtDevices.hpp"

#include "core/graphics/VtSubpass.hpp"

struct VtRenderpassAttachmentInfo
{
  VkFormat              format;
  VkSampleCountFlagBits samples        = VK_SAMPLE_COUNT_1_BIT;
  VkAttachmentLoadOp    loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
  VkAttachmentStoreOp   storeOp        = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  VkAttachmentLoadOp    stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  VkAttachmentStoreOp   stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  VkImageLayout         initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
  VkImageLayout         finalLayout    = VK_IMAGE_LAYOUT_UNDEFINED;
};

struct VtRenderpassCreateInfo
{
		std::string name{"NULL"};
		VtDevices*  pVtDevices{nullptr};
		VtSubpass*  pVtSubpass{nullptr};

		std::vector<VtRenderpassAttachmentInfo> attachments;
};

class VtRenderpass
{
	public:
		VtRenderpass(VtRenderpassCreateInfo _createInfo);
    VtRenderpass(const VtRenderpass&)=delete;
    VtRenderpass()=default;
    ~VtRenderpass();

    VtRenderpass& operator=(const VtRenderpass&) = delete;
    VtRenderpass& operator=(VtRenderpass&& other) noexcept;
    VtRenderpass(VtRenderpass&& other) noexcept;

    inline auto& getInstance() const noexcept { return m_renderpass; };

	private:
		std::string  m_name{"NULL"};
		VtDevices*   m_pVtDevices{nullptr};
    VkRenderPass m_renderpass{nullptr};
};
