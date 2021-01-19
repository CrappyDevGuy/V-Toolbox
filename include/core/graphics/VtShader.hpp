//_Author : CrappyDevGuy, M01D18Y2021_//

#pragma once

#include "core/VtDevices.hpp"

struct VtShaderInput
{
  std::vector<VkVertexInputBindingDescription> bindings;
  std::vector<VkVertexInputAttributeDescription> attributes;

  void addInput(std::uint32_t t_binding, std::uint32_t t_location, VkFormat t_dataForm, std::uint32_t t_offset)
  {
    attributes.push_back(VkVertexInputAttributeDescription{});
    attributes.back().binding  = t_binding;
    attributes.back().location = t_location;
    attributes.back().format   = t_dataForm;
    attributes.back().offset   = t_offset;
  };

  void addBinding(std::uint32_t t_binding, VkVertexInputRate t_inputRate, std::uint32_t t_inputSize)
  {
    bindings.push_back(VkVertexInputBindingDescription{});
    bindings.back().binding   = t_binding;
    bindings.back().stride    = t_inputSize;
    bindings.back().inputRate = t_inputRate;
  };

  std::vector<VkVertexInputBindingDescription>&   getBindingInfo()    { return bindings; };
  std::vector<VkVertexInputAttributeDescription>& getAttributesInfo() { return attributes; };

  std::uint32_t getAttributesSize() { return attributes.size(); };
  std::uint32_t getBindingsSize()   { return bindings.size();   };
  bool          isEmpty()           { return attributes.size() <= 0 && bindings.size() <= 0; };

};

struct VtShaderCreateInfo
{
	std::string name{"NULL"};
	
  std::string vertShader{"NULL"};
  std::string fragShader{"NULL"};
  std::string geomShader{"NULL"};

	VtDevices* pVtDevices{nullptr};

	VtShaderInput shaderInput;
};

class VtShader
{
	public:
		VtShader(VtShaderCreateInfo _createInfo);
		VtShader(const VtShader&) = delete;
		VtShader() = default;
		~VtShader();

		VtShader& operator=(const VtShader&) = delete;
		VtShader& operator=(VtShader&& other) noexcept;
		VtShader(VtShader&& other) noexcept;

		
		inline auto& getVertexShader()   const noexcept { return m_vertShader;  };
    inline auto& getFragmentShader() const noexcept { return m_fragShader;  };
    inline auto& getGeometryShader() const noexcept { return m_geomShader;  };
    inline auto& getShaderInput()          noexcept { return m_shaderInput; };
		
    VkPipelineShaderStageCreateInfo getVertexShaderStage();
    VkPipelineShaderStageCreateInfo getFragmentShaderStage();
    VkPipelineShaderStageCreateInfo getGeometryShaderStage();

    VkPipelineVertexInputStateCreateInfo getPipelineVertexInputState();

	private:
		void createShaderModule(VkShaderModule& module, const std::string type, const std::vector<char>& code);
		void destroyShaderModule(VkShaderModule* module, const std::string type);

	private:
		std::string m_name{"NULL"};
		VtDevices*  m_pVtDevices{nullptr};

    VkShaderModule m_vertShader{nullptr};
    VkShaderModule m_fragShader{nullptr};
    VkShaderModule m_geomShader{nullptr};	

    VtShaderInput m_shaderInput{};
};
