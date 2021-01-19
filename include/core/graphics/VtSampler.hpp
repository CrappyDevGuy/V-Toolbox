//_Author : CrappyDevGuy, M01D19Y2021_//

#pragma once

#include "core/VtDevices.hpp"

struct VtSamplerCreateInfo
{
	std::string name{"NULL"};
	VtDevices*  pVtDevices{nullptr};
};

class VtSampler
{
	struct InternalData
	{
		VkFilter             textureFilter   = VK_FILTER_LINEAR;
    VkSamplerAddressMode textureWrap     = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    float                anisotropyLevel = 1; // set to 0 for disable it
    VkBorderColor        borderColor     = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    std::uint32_t        mipLevels       = 1000;
	};

	public:
		VtSampler(VtSamplerCreateInfo _createInfo);
		VtSampler(const VtSampler&) = delete;
		VtSampler() = default;
		~VtSampler();

		VtSampler& operator=(const VtSampler&) = delete;
		VtSampler& operator=(VtSampler&& other) noexcept;
		VtSampler(VtSampler&& other) noexcept;

		void build();

		inline void setFilter(VkFilter v) 							noexcept { m_data.textureFilter   = v; };
		inline void setWrapMode(VkSamplerAddressMode v) noexcept { m_data.textureWrap     = v; };
		inline void setAnisotropicLevel(float v)				noexcept { m_data.anisotropyLevel = v; };
		inline void setBorderColor(VkBorderColor v)			noexcept { m_data.borderColor     = v; };
		inline void setMipMapLevel(std::uint32_t v)			noexcept { m_data.mipLevels       = v; };

		inline auto& getInstance() noexcept { return m_sampler; };

	private:
		std::string m_name{"NULL"};
		VtDevices*  m_pVtDevices{nullptr};
    VkSampler 	m_sampler{nullptr};

    VtSampler::InternalData m_data{};
};
