//_Author : CrappyDevGuy, M12D09Y2020_//

#pragma once

#include <string>
#include <filesystem>
#include <set>
#include <vector>
#include "volk.h"

namespace VtUtil
{

	bool checkVulkanResult(std::string name, VkResult result);
	std::string VkQueueFlagsToString(VkQueueFlags flags);
	std::string VkSampleFlagsToString(VkSampleCountFlagBits flag);
	std::string VkCommandPoolFlagsToString(VkCommandPoolCreateFlags flag);
	std::string VkDescriptorTypeFlagToString(VkDescriptorType flag);

  namespace file
  {
    void load(const std::string& filepath, std::vector<char>& datas);
    void getFiles(std::string& directory, std::set<std::string>& files);
    void getFiles(std::string& directory, std::vector<std::string>& files);
    
    inline bool exist(std::string file)       { return std::filesystem::exists(file);       };
    inline bool isDirectory(std::string file) { return std::filesystem::is_directory(file); };
  }	
}
