//_Author : CrappyDevGuy, M01D8Y2021_//

#pragma once

#include "core/entities/VtEntity.hpp"

struct VtCameraCreateInfo
{
	glm::vec3 position{0.0f};
  glm::vec3 rotation{0.0f};
  glm::vec2 zValue{0.01f, 500.0f};
};

class VtCamera : public VtEntity
{
	public:
		VtCamera(VtCameraCreateInfo _createInfo);
		VtCamera(const VtCamera&) = delete;
		VtCamera() = default;
		~VtCamera() {}

		VtCamera& operator=(const VtCamera&) = delete;
		VtCamera& operator=(VtCamera&& other) noexcept;
		VtCamera(VtCamera&& other) noexcept;

    void createOrthographicMatrix(float planeX, float planeY);
    void createPerspectiveMatrix(float fov, float width, float height);
    void createViewMatrix();

    inline auto& getRefProjectionMatrix()  noexcept { return m_projMatrix;            };
    inline auto  getCopyProjectionMatrix() noexcept { return glm::mat4{m_projMatrix}; };

    inline auto& getRefViewMatrix()  noexcept { return m_viewMatrix;            };
    inline auto  getCopyViewMatrix() noexcept { return glm::mat4{m_viewMatrix}; };

	private:
    glm::vec2 m_zValue{0.01f, 500.0f};

    glm::mat4 m_viewMatrix{0.0f};
    glm::mat4 m_projMatrix{0.0f};
	
};
