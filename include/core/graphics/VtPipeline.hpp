//_Author : CrappyDevGuy, M01D18Y2021_//

#pragma once

#include "core/VtDevices.hpp"
#include "core/graphics/VtShader.hpp"
#include "core/graphics/layout/VtPipelineLayout.hpp"
#include "core/graphics/VtRenderpass.hpp"

struct VtPipelineCreateInfo
{
	std::string name{"NULL"};
	VtDevices* pVtDevices{nullptr};
};

class VtPipeline
{
	struct InternalData
	{
		
    glm::ivec2      			viewport    = {800,600};
    VkBool32        			blend       = VK_FALSE;
      
    VkCullModeFlags       cullFace    = VK_CULL_MODE_FRONT_BIT;
    VkPolygonMode         polygonMode = VK_POLYGON_MODE_FILL;
    float                 lineWidth   = 1.0f;
    VkSampleCountFlagBits msaaValue   = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineBindPoint		bindPoint	  = VK_PIPELINE_BIND_POINT_GRAPHICS;
	};

	public:
		VtPipeline(VtPipelineCreateInfo _createInfo);
		VtPipeline(const VtPipeline&) = delete;
		VtPipeline() = default;
		~VtPipeline();

		VtPipeline& operator=(const VtPipeline&) = delete;
		VtPipeline& operator=(VtPipeline&& other) noexcept;
		VtPipeline(VtPipeline&& other) noexcept;


		inline void setSamples(VkSampleCountFlagBits v) noexcept { m_parameters.msaaValue = v; };
		inline void setCullFace(VkCullModeFlags v)      noexcept { m_parameters.cullFace  = v; };
		inline void setViewport(glm::ivec2 v)						noexcept { m_parameters.viewport  = v; };
		inline void setBindPoint(VkPipelineBindPoint v) noexcept { m_parameters.bindPoint = v; };

		inline auto& getInstance()  const noexcept { return m_pipeline; 						};
		inline auto  getBindPoint() const noexcept { return m_parameters.bindPoint; };

		void build(VtShader* pVtShader, VtRenderpass* pVtRenderpass, VtPipelineLayout* pVtPipelineLayout);

	private:
		std::string m_name{"NULL"};
		VtDevices*  m_pVtDevices{nullptr};
    VkPipeline  m_pipeline{nullptr};

    VtPipeline::InternalData m_parameters;
};
