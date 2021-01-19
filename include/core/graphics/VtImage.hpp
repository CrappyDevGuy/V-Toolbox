//_Author : CrappyDevGuy, M12D29Y2020_//

#pragma once

#include "core/VtDevices.hpp"

struct VtImageCreateInfo
{
	std::string name{"NULL"};
	VtDevices*  pVtDevices{nullptr};
  VkFormat    format;
};

struct VtImageInfo
{
  VkImageUsageFlags     usage;
  glm::ivec2            extent        = glm::ivec2{0,0};
  VkSampleCountFlagBits samplesCount  = VK_SAMPLE_COUNT_1_BIT;
  bool                  createMipMaps = false;
  VkImageTiling         tiling        = VK_IMAGE_TILING_OPTIMAL;
  VkImageLayout         initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
};

class VtImage
{
	struct InternalData
	{
		std::string   		 	  name;
		bool 							 	  hasBeenSet   = true;
		bool 							 	  hasMipMaps   = false;
    VkSampleCountFlagBits samplesCount = VK_SAMPLE_COUNT_1_BIT;
    std::uint32_t      		mipLevels    = 1;
    std::uint32_t         channelCount = 1;
    VkFormat           	  format;
    VkImageAspectFlags 	  aspect;    
    glm::ivec2 						extent{800, 600};
	};

	public:
		VtImage(VtImageCreateInfo _createInfo);
		VtImage(const VtImage&) = delete;
		VtImage() = default;
		~VtImage();

		VtImage& operator=(const VtImage&) = delete;
		VtImage& operator=(VtImage&& other) noexcept;
		VtImage(VtImage&& other) noexcept;

		void createImage(VtImageInfo _imageInfo);
    void createView(VkImageAspectFlags aspect);
    void createMemory(VkMemoryPropertyFlags t_properties);

    void transitionLayout(VkCommandBuffer& commandBuffer, VkImageLayout t_oldLayout, VkImageLayout t_newLayout);
    void loadImage(VkCommandBuffer& commandBuffer, std::string path);
    void generateMipMaps(VkCommandBuffer& commandBuffer);

    inline void setImage(VkImage image) noexcept
    { m_image = std::move(image); };
    
    inline VkImage&           getImage()        		  noexcept { return m_image;             };
    inline VkImageView&       getImageView()    		  noexcept { return m_imageView;         };
    inline VkDeviceMemory&    getImageMemory()  		  noexcept { return m_imageMemory;       };
    inline std::string_view   getName()         const noexcept { return m_data.name;         };
    inline glm::ivec2         getExtent()       const noexcept { return m_data.extent;       };
    inline VkImageAspectFlags getAspect()       const noexcept { return m_data.aspect;       };
    inline std::uint32_t      getChannelCount() const noexcept { return m_data.channelCount; };

	private:
		VtImage::InternalData m_data{};
		
    VtDevices* m_pVtDevices{nullptr};
	
    VkImage        m_image{nullptr};
    VkImageView    m_imageView{nullptr};
    VkDeviceMemory m_imageMemory{nullptr};
};
