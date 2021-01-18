//_Author : CrappyDevGuy, M01D18Y2021_//

#include "core/graphics/VtSubpass.hpp"
#include "core/io/VtLogHandler.hpp"

#include <cassert>

VtSubpass& VtSubpass::operator=(VtSubpass&& other) noexcept
{
	std::swap(other.m_name, m_name);
 	std::swap(other.m_dependencies, m_dependencies);
 	std::swap(other.m_descriptions, m_descriptions);
 	return *this;
}

VtSubpass::VtSubpass(VtSubpass&& other) noexcept
:m_name{std::move(other.m_name)}
,m_dependencies{std::move(other.m_dependencies)}
,m_descriptions{std::move(other.m_descriptions)}
{}

VtSubpass::VtSubpass(std::string name)
{
	m_name = std::move(name);
	VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::VtSubpass", "Success to create");	
}

VtSubpass::~VtSubpass()
{
  if(m_name != "NULL")
  {
   	VtLogHandler::oStreamDebug("V-Toolbox", m_name+"::VtSubpass", "Success to destroy");	
    m_dependencies.clear();
    m_descriptions.clear();
  }
}

void VtSubpass::addDescription(VtSubpassDescription& t_description)
{
  assert(t_description.hasBindPoint == true);

  VkSubpassDescription c = {};
  c.pipelineBindPoint       = t_description.pipelineBindPoint;
  c.pDepthStencilAttachment = &t_description.depthReference;

  int dataCount = 0;

  if(!t_description.colorReferences.empty())
  {
    c.colorAttachmentCount = static_cast<std::uint32_t>(t_description.colorReferences.size());
    c.pColorAttachments    = t_description.colorReferences.data();
    dataCount++;
  }

  if(!t_description.inputReferences.empty())
  {
    c.inputAttachmentCount = static_cast<std::uint32_t>(t_description.inputReferences.size());
    c.pInputAttachments    = t_description.inputReferences.data();
    dataCount++;
  }

  if(!t_description.resolveReferences.empty())
  {
    c.pResolveAttachments    = t_description.resolveReferences.data();
    dataCount++;
  }

  if(dataCount == 0 && t_description.depthReference.attachment == VK_ATTACHMENT_UNUSED)
  	VtLogHandler::oStreamWarning("V-Toolbox", m_name+"::VtSubpass::addDescriptor", "no data found");

  m_descriptions.push_back(c);
}

void VtSubpass::addDescriptions(std::vector<VtSubpassDescription>& t_descriptions)
{
  std::uint32_t vectorSize = m_descriptions.size();
  m_descriptions.reserve(vectorSize+t_descriptions.size());

  for(auto c : t_descriptions)
    addDescription(c);
}

void VtSubpass::addDependency(VkSubpassDependency& t_dependency)
{
  m_dependencies.push_back(t_dependency);
}

void VtSubpass::addDependencies(std::vector<VkSubpassDependency>& t_dependencies)
{
  std::uint32_t vectorSize = m_dependencies.size();
  m_dependencies.reserve(vectorSize+t_dependencies.size());

  for(auto c : t_dependencies)
    addDependency(c);
}
