//_Author : CrappyDevGuy, M12D29Y2020_//

#pragma once

#include "core/VtWindow.hpp"
#include "core/VtDevices.hpp"
#include "core/graphics/VtImage.hpp"


struct VtSwapchainCreateInfo
{
	std::string name{"NULL"};

	VtWindow*   pVtWindow{nullptr};
	VtDevices*  pVtDevices{nullptr};
	
  VkPresentModeKHR presentMode{VK_PRESENT_MODE_MAILBOX_KHR};
};

class VtSwapchain
{
	struct InternalData
	{
		std::string name;

		VkFormat    imageFormat,
						    depthFormat;
		glm::ivec2  extent;

		VtImage 						 colorImage,
							  				 depthImage;
		std::vector<VtImage> imagesView;
	};
	
	public:
		VtSwapchain(VtSwapchainCreateInfo _createInfo);
		VtSwapchain(const VtSwapchain&) = delete;
		VtSwapchain() = default;
		~VtSwapchain();

		VtSwapchain& operator=(const VtSwapchain&) = delete;
		VtSwapchain& operator=(VtSwapchain&& other) noexcept;
		VtSwapchain(VtSwapchain&& other) noexcept;
		

		VkResult acquireImage(VkSemaphore& t_semaphore, std::uint32_t& imageIndex);

	private:
    VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availables);
    VkPresentModeKHR   choosePresentMode(const std::vector<VkPresentModeKHR>& availables, VkPresentModeKHR present);
    VkExtent2D 				 chooseExtent(glm::ivec2 extent, const VkSurfaceCapabilitiesKHR& capabilities);
    std::uint32_t 		 getImageCount(const VkSurfaceCapabilitiesKHR& capabilities);

    void createImageViews();
    void createImages();
    
	private:
    VkSwapchainKHR            m_swapchain{nullptr};
    VtDevices*							  m_pVtDevices{nullptr};

    VtSwapchain::InternalData m_data{};
};
