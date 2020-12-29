//_Author : CrappyDevGuy, M12D09Y2020_//

#include "core/io/VtLogHandler.hpp"
#include "core/util/VtUtil.hpp"

#include <fstream>
#include <sstream>

std::string VtUtil::VkQueueFlagsToString(VkQueueFlags flags)
{
    std::string strflags = "";
    if(flags & VK_QUEUE_GRAPHICS_BIT)       strflags += "Graphics ";
    if(flags & VK_QUEUE_COMPUTE_BIT)        strflags += "Compute ";
    if(flags & VK_QUEUE_TRANSFER_BIT)       strflags += "Transfer ";
    if(flags & VK_QUEUE_SPARSE_BINDING_BIT) strflags += "SparseBinding ";
    if(flags & VK_QUEUE_PROTECTED_BIT)      strflags += "Protected ";
    return strflags;
}

std::string VtUtil::VkSamplesFlagToString(VkSampleCountFlagBits flag)
{
    std::string strflag = "";
        if(flag & VK_SAMPLE_COUNT_64_BIT) strflag += "VK_SAMPLE_COUNT_64_BIT";
    else if(flag & VK_SAMPLE_COUNT_32_BIT) strflag += "VK_SAMPLE_COUNT_32_BIT";
    else if(flag & VK_SAMPLE_COUNT_16_BIT) strflag += "VK_SAMPLE_COUNT_16_BIT";
    else if(flag & VK_SAMPLE_COUNT_8_BIT)  strflag += "VK_SAMPLE_COUNT_8_BIT";
    else if(flag & VK_SAMPLE_COUNT_4_BIT)  strflag += "VK_SAMPLE_COUNT_4_BIT";
    else if(flag & VK_SAMPLE_COUNT_2_BIT)  strflag += "VK_SAMPLE_COUNT_2_BIT";
    else if(flag & VK_SAMPLE_COUNT_1_BIT)  strflag += "VK_SAMPLE_COUNT_1_BIT";
    return strflag;
}
    
bool VtUtil::checkVulkanResult(std::string name, VkResult result)
{

	#ifndef VT_DEBUG_DEFINE
		if(result != VK_SUCCESS)
		{
			VtLogHandler::oStreamFatalError("V-Toolbox", "VtUtil::checkVulkanResult", name + " VkResult : !VK_SUCCESS");
		}
	#endif

  switch(result)
  {
   case VK_NOT_READY :
        VtLogHandler::oStreamWarning("V-Toolbox", "VtUtil::checkVulkanResult", name + " VkResult : VK_NOT_READY");
        break;
   case VK_TIMEOUT :
        VtLogHandler::oStreamWarning("V-Toolbox", "VtUtil::checkVulkanResult", name + " VkResult : VK_TIMEOUT");
        break;
   case VK_EVENT_SET :
        VtLogHandler::oStreamWarning("V-Toolbox", "VtUtil::checkVulkanResult", name + " VkResult : VK_EVENT_SET");
        break;
   case VK_EVENT_RESET :
        VtLogHandler::oStreamWarning("V-Toolbox", "VtUtil::checkVulkanResult", name + " VkResult : VK_EVENT_RESET");
        break;
   case VK_INCOMPLETE :
        VtLogHandler::oStreamWarning("V-Toolbox", "VtUtil::checkVulkanResult", name + " VkResult : VK_INCOMPLETE");
        break;
   case VK_SUBOPTIMAL_KHR :
        VtLogHandler::oStreamWarning("V-Toolbox", "VtUtil::checkVulkanResult", name + " VkResult : VK_SUBOPTIMAL_KHR");
        break;
   case VK_ERROR_OUT_OF_HOST_MEMORY :
        VtLogHandler::oStreamFatalError("V-Toolbox", "VtUtil::checkVulkanResult", name + " VkResult : VK_ERROR_OUT_OF_HOST_MEMORY");
        break;
   case VK_ERROR_OUT_OF_DEVICE_MEMORY :
        VtLogHandler::oStreamFatalError("V-Toolbox", "VtUtil::checkVulkanResult", name + " VkResult : VK_ERROR_OUT_OF_DEVICE_MEMORY");
        break;
   case VK_ERROR_INITIALIZATION_FAILED :
        VtLogHandler::oStreamFatalError("V-Toolbox", "VtUtil::checkVulkanResult", name + " VkResult : VK_ERROR_INITIALIZATION_FAILED");
        break;
   case VK_ERROR_DEVICE_LOST :
        VtLogHandler::oStreamFatalError("V-Toolbox", "VtUtil::checkVulkanResult", name + " VkResult : VK_ERROR_DEVICE_LOST");
        break;
   case VK_ERROR_MEMORY_MAP_FAILED :
        VtLogHandler::oStreamFatalError("V-Toolbox", "VtUtil::checkVulkanResult", name + " VkResult : VK_ERROR_MEMORY_MAP_FAILED");
        break;
   case VK_ERROR_LAYER_NOT_PRESENT :
        VtLogHandler::oStreamFatalError("V-Toolbox", "VtUtil::checkVulkanResult", name + " VkResult : VK_ERROR_LAYER_NOT_PRESENT");
        break;
   case VK_ERROR_EXTENSION_NOT_PRESENT :
        VtLogHandler::oStreamFatalError("V-Toolbox", "VtUtil::checkVulkanResult", name + " VkResult : VK_ERROR_EXTENSION_NOT_PRESENT");
        break;
   case VK_ERROR_FEATURE_NOT_PRESENT :
        VtLogHandler::oStreamFatalError("V-Toolbox", "VtUtil::checkVulkanResult", name + " VkResult : VK_ERROR_FEATURE_NOT_PRESENT");
        break;
   case VK_ERROR_INCOMPATIBLE_DRIVER :
        VtLogHandler::oStreamFatalError("V-Toolbox", "VtUtil::checkVulkanResult", name + " VkResult : VK_ERROR_INCOMPATIBLE_DRIVER");
        break;
   case VK_ERROR_TOO_MANY_OBJECTS :
        VtLogHandler::oStreamFatalError("V-Toolbox", "VtUtil::checkVulkanResult", name + " VkResult : VK_ERROR_TO_MANY_OBJECTS");
        break;
   case VK_ERROR_FORMAT_NOT_SUPPORTED :
        VtLogHandler::oStreamFatalError("V-Toolbox", "VtUtil::checkVulkanResult", name + " VkResult : VK_ERROR_FORMAT_NOT_SUPPORTED");
        break;
   case VK_ERROR_FRAGMENTED_POOL :
        VtLogHandler::oStreamFatalError("V-Toolbox", "VtUtil::checkVulkanResult", name + " VkResult : VK_ERROR_FRAGMENTED_POOL");
        break;
   case VK_ERROR_OUT_OF_POOL_MEMORY :
        VtLogHandler::oStreamFatalError("V-Toolbox", "VtUtil::checkVulkanResult", name + " VkResult : VK_ERROR_OUT_OF_POOL_MEMORY");
        break;
   case VK_ERROR_INVALID_EXTERNAL_HANDLE :
        VtLogHandler::oStreamFatalError("V-Toolbox", "VtUtil::checkVulkanResult", name + " VkResult : VK_ERROR_INVALID_EXTERNAL_HANDLE");
        break;
   case VK_ERROR_SURFACE_LOST_KHR :
        VtLogHandler::oStreamFatalError("V-Toolbox", "VtUtil::checkVulkanResult", name + " VkResult : VK_ERROR_SURFACE_LOST_KHR");
        break;
   case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR :
        VtLogHandler::oStreamFatalError("V-Toolbox", "VtUtil::checkVulkanResult", name + " VkResult : VK_ERROR_NATIVE_WINDOW_IN_USE_KHR");
        break;
   case VK_ERROR_OUT_OF_DATE_KHR :
        VtLogHandler::oStreamFatalError("V-Toolbox", "VtUtil::checkVulkanResult", name + " VkResult : VK_ERROR_OUT_OF_DATE_KHR");
        break;
   case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR :
        VtLogHandler::oStreamFatalError("V-Toolbox", "VtUtil::checkVulkanResult", name + " VkResult : VK_ERROR_INCOMPATIBLE_DISPLAY_KHR");
        break;
   case VK_ERROR_VALIDATION_FAILED_EXT :
        VtLogHandler::oStreamFatalError("V-Toolbox", "VtUtil::checkVulkanResult", name + " VkResult : VK_ERROR_VALIDATION_FAILED_EXT");
        break;
   case VK_ERROR_INVALID_SHADER_NV :
        VtLogHandler::oStreamFatalError("V-Toolbox", "VtUtil::checkVulkanResult", name + " VkResult : VK_ERROR_INVALID_SHADER_NV");
        break;
   case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT :
        VtLogHandler::oStreamFatalError("V-Toolbox", "VtUtil::checkVulkanResult", name + " VkResult : VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT");
        break;
   case VK_ERROR_NOT_PERMITTED_EXT :
        VtLogHandler::oStreamFatalError("V-Toolbox", "VtUtil::checkVulkanResult", name + " VkResult : VK_ERROR_NOT_PERMITTED_EXT");
        break;
   case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT :
        VtLogHandler::oStreamFatalError("V-Toolbox", "VtUtil::checkVulkanResult", name + " VkResult : VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT");
        break;
   case VK_ERROR_FRAGMENTATION_EXT :
        VtLogHandler::oStreamFatalError("V-Toolbox", "VtUtil::checkVulkanResult", name + " VkResult : VK_ERROR_FRAGMENTATION_EXT");
        break;
   case VK_ERROR_INVALID_DEVICE_ADDRESS_EXT :
        VtLogHandler::oStreamFatalError("V-Toolbox", "VtUtil::checkVulkanResult", name + " VkResult : VK_ERROR_INVALID_DEVICE_ADDRESS_EXT");
        break;
   case VK_RESULT_MAX_ENUM :
        VtLogHandler::oStreamFatalError("V-Toolbox", "VtUtil::checkVulkanResult", name + " VkResult : VK_RESULT_MAX_ENUM");
        break;
    default: break;
  }

  return true;   

}

namespace dcafs = std::filesystem;
void VtUtil::file::load(const std::string& filepath, std::vector<char>& datas)
{
  std::ifstream file(filepath, std::ios::ate | std::ios::binary);

  if(!file.is_open())
  	VtLogHandler::oStreamFatalError("V-Toolbox", "VtUtil::file::load", "Failed to open : " + filepath);

  std::size_t fileSize = (size_t) file.tellg();
  datas.resize(fileSize);

  file.seekg(0);
  file.read(datas.data(), fileSize);

  file.close();
  VtLogHandler::oStream("V-Toolbox", "VtUtil::file::load", "Success to load : " + filepath);
}

void VtUtil::file::getFiles(std::string& directory, std::set<std::string>& files)
{
  for(const auto& entry : dcafs::directory_iterator(directory))
    files.insert(entry.path().c_str());
}

void VtUtil::file::getFiles(std::string& directory, std::vector<std::string>& files)
{
  for(const auto& entry : dcafs::directory_iterator(directory))
    files.push_back(entry.path().c_str());
}
