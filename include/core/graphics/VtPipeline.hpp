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
	struct InternalState
	{
	  VkPipelineRasterizationStateCreateInfo razCreate{};
	  VkPipelineMultisampleStateCreateInfo   msCreate{};
		VkPipelineDepthStencilStateCreateInfo  depthStencil{};
	  VkPipelineColorBlendAttachmentState    colorBlendAttachment{};
		VkPipelineColorBlendStateCreateInfo    colorBlending{};
		VkPipelineInputAssemblyStateCreateInfo inAsmCreate{};

		VkRect2D 						scissor{};
		VkViewport 					viewport{};
		VkPipelineBindPoint bindPoint;
	};

	public:
		VtPipeline(VtPipelineCreateInfo _createInfo);
		VtPipeline(const VtPipeline&) = delete;
		VtPipeline() = default;
		~VtPipeline();

		VtPipeline& operator=(const VtPipeline&) = delete;
		VtPipeline& operator=(VtPipeline&& other) noexcept;
		VtPipeline(VtPipeline&& other) noexcept;

		inline auto& getInstance()  const noexcept { return m_pipeline; 			  };
		inline auto  getBindPoint() const noexcept { return m_states.bindPoint; };
				
		void build(VtShader* pVtShader, VtRenderpass* pVtRenderpass, VtPipelineLayout* pVtPipelineLayout);

		void setViewport(glm::ivec2 viewport)    noexcept;
		void setViewport(VkExtent2D viewport)    noexcept;
		void setScissorExtent(glm::ivec2 extent) noexcept;
		void setScissorExtent(VkExtent2D extent) noexcept;
		
		void setPolygonMode(VkPolygonMode v)			    noexcept { m_states.razCreate.polygonMode 			  = v; };
		void setSamplesCount(VkSampleCountFlagBits v) noexcept { m_states.msCreate.rasterizationSamples = v; };
		void setCullMode(VkCullModeFlags v)			 			noexcept { m_states.razCreate.cullMode 	  			  = v; };
		void setBindPoint(VkPipelineBindPoint v) 			noexcept { m_states.bindPoint 				 	  			  = v; };

	private:
		void populateStateStructure();

	private:
		std::string m_name{"NULL"};
		VtDevices*  m_pVtDevices{nullptr};
    VkPipeline  m_pipeline{nullptr};

    VtPipeline::InternalState m_states{};
};
