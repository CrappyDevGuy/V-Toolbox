//_Author : CrappyDevGuy, M01D18Y2021_//

#pragma once

#include "core/VtDevices.hpp"
#include "core/graphics/VtShader.hpp"

struct VtPipelineCreateInfo
{
	std::string name{"NULL"};
	VtDevices* pVtDevices{nullptr};
};

class VtPipeline
{
	struct InternalData
	{
		
	};

	public:
		VtPipeline(VtPipelineCreateInfo _createInfo);
		VtPipeline(const VtPipeline&) = delete;
		VtPipeline() = default;
		~VtPipeline();

		VtPipeline& operator=(const VtPipeline&) = delete;
		VtPipeline& operator=(VtPipeline&& other) noexcept;
		VtPipeline(VtPipeline&& other) noexcept;

		inline auto& getInstance() const noexcept { return m_pipeline; };

		void build(VtShader* pShader);

	private:
		std::string m_name{"NULL"};
		VtDevices*  m_pVtDevices{nullptr};
    VkPipeline  m_pipeline{nullptr};
};
