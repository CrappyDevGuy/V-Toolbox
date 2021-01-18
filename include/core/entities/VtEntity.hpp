//_Author : CrappyDevGuy, M01D18Y2021_//

#pragma once

#include <glm/glm.hpp>

class VtEntity
{
  public:
	  VtEntity(glm::vec3 position, glm::vec3 rotation)
	  :m_position{std::move(position)}
	  ,m_rotation{std::move(rotation)}
	  {}
	    
	  VtEntity(glm::vec3 position)
	  :m_position{std::move(position)}
	  {}

	  VtEntity(const VtEntity&) = delete;
	  VtEntity()=default;
	  ~VtEntity() {}

	  VtEntity& operator=(const VtEntity&) = delete;
	  VtEntity& operator=(VtEntity&& other) noexcept
	  { 
	    std::swap(other.m_position, m_position);
	    std::swap(other.m_rotation, m_rotation);
	    return *this; 
	  }

	  VtEntity(VtEntity&& other) noexcept
	  :m_position{std::move(m_position)}
	  ,m_rotation{std::move(m_rotation)}
	  {}

	  inline void incPosition(glm::vec3 position) noexcept { m_position += position; };
	  inline void incRotation(glm::vec3 rotation) noexcept { m_rotation += rotation; };

	  inline void decPosition(glm::vec3 position) noexcept { m_position -= position; };
	  inline void decRotation(glm::vec3 rotation) noexcept { m_rotation -= rotation; };

	  inline void setPosition(glm::vec3 position) noexcept { m_position = std::move(position); };
	  inline void setRotation(glm::vec3 rotation) noexcept { m_rotation = std::move(rotation); };

	  inline auto& getRefPosition() noexcept { return m_position; };
	  inline auto& getRefRotation() noexcept { return m_rotation; };

	  inline auto getCopyPosition() noexcept { return glm::vec3{m_position}; };
	  inline auto getCopyRotation() noexcept { return glm::vec3{m_rotation}; };

	private:
	  glm::vec3 m_position{0.0f};
  	glm::vec3 m_rotation{0.0f};

};
