//_Author : CrappyDevGuy, M01D18Y2021_//

#pragma once

#include <string>
#include <vector>
#include "volk.h"

struct VtSubpassDescription
{
  std::string name{"NULL"};
  std::vector<VkAttachmentReference> colorReferences;
  std::vector<VkAttachmentReference> inputReferences;
  std::vector<VkAttachmentReference> resolveReferences;
  VkAttachmentReference depthReference{VK_ATTACHMENT_UNUSED};
  VkPipelineBindPoint   pipelineBindPoint;

  bool hasBindPoint = false;

  inline void setPipelineBindPoint(VkPipelineBindPoint point) { pipelineBindPoint = point; hasBindPoint = true; };
  inline void addColorReference(VkAttachmentReference _ref)   { colorReferences.push_back(_ref); };
  inline void addInputReference(VkAttachmentReference _ref)   { inputReferences.push_back(_ref); };
  inline void addResolveReference(VkAttachmentReference _ref) { resolveReferences.push_back(_ref); };
  inline void addDepthReference(VkAttachmentReference _ref)   { depthReference = _ref; };
};

class VtSubpass
{
	public:
    VtSubpass(std::string name);
    VtSubpass(const VtSubpass&)=delete;
    VtSubpass()=default;
    ~VtSubpass();

    VtSubpass& operator=(const VtSubpass&) = delete;
    VtSubpass& operator=(VtSubpass&& other) noexcept;
    VtSubpass(VtSubpass&& other) noexcept;

    void addDescription(VtSubpassDescription& t_description);
    void addDescriptions(std::vector<VtSubpassDescription>& t_descriptions);

    void addDependency(VkSubpassDependency& t_dependency);
    void addDependencies(std::vector<VkSubpassDependency>& t_dependencies);

    inline std::vector<VkSubpassDependency>  getDependencies() const noexcept { return m_dependencies; }; 
    inline std::vector<VkSubpassDescription> getDescriptions() const noexcept { return m_descriptions; };  
    inline std::string_view                  getName()         const noexcept { return m_name; };

	private:
		std::string 											m_name{"NULL"};
    std::vector<VkSubpassDependency>  m_dependencies;
    std::vector<VkSubpassDescription> m_descriptions;
};
