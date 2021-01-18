//_Author : CrappyDevGuy, M12D09Y2020_//

#pragma once

#include <string>
#include <filesystem>
#include <set>
#include <vector>
#include "volk.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

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

  namespace chrono
  {
    using chronoType = std::chrono::time_point<std::chrono::system_clock>;
    inline chronoType step() {return std::chrono::system_clock::now();};
    template<typename T>
    inline double elapsedTime(chronoType start, chronoType end)
    {
      return std::chrono::duration_cast<T>(end-start).count();
    };
  }

  namespace maths
  {
    glm::mat4 createPerspectiveMatrix(float fov, float width, float height, float near,	float far);
    glm::mat4 createOrthographicMatrix(float planeX, float planeY, float near, float far);
    glm::mat4 createViewMatrix(glm::vec3 position, glm::vec3 front, glm::vec3 up);
    glm::mat4 createViewMatrix(glm::vec3 position, glm::vec3 rot);
    glm::mat4 createTransformationMatrix(glm::vec3 translation, glm::vec3 rot, float sc);
    glm::mat4 createTransformationMatrix(glm::vec3 translation, glm::vec3 rot, glm::vec3 sc);
    glm::vec3 getFront(glm::vec3 const rotation);
    glm::vec3 getRight(glm::vec3 const rotation);
    glm::vec3 getUp(glm::vec3 const rotation);
    glm::vec3 getLeft(glm::vec3 const rotation);

    glm::vec2 posOnCircle(glm::vec2 center, float radius, float time);
  }
}
