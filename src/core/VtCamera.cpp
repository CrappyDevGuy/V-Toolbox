//_Author : CrappyDevGuy, M01D8Y2021_//

#include "core/view/VtCamera.hpp"

#include "core/util/VtUtil.hpp"

VtCamera& VtCamera::operator=(VtCamera&& other) noexcept
{
	std::swap(other.m_zValue, m_zValue);
	std::swap(other.m_viewMatrix, m_viewMatrix);
	std::swap(other.m_projMatrix, m_projMatrix);
	return *this;
}

VtCamera::VtCamera(VtCamera&& other) noexcept
:m_zValue{std::move(other.m_zValue)}
,m_viewMatrix{std::move(other.m_viewMatrix)}
,m_projMatrix{std::move(other.m_projMatrix)}
{}

VtCamera::VtCamera(VtCameraCreateInfo _createInfo)
:VtEntity{_createInfo.position, _createInfo.rotation}
{
	m_zValue = std::move(_createInfo.zValue);
}

void VtCamera::createOrthographicMatrix(float planeX, float planeY)
{
  m_projMatrix = VtUtil::maths::createOrthographicMatrix(planeX, planeY, m_zValue.x, m_zValue.y);
}

void VtCamera::createPerspectiveMatrix(float fov, float width, float height)
{
  m_projMatrix = VtUtil::maths::createPerspectiveMatrix(fov, width, height, m_zValue.x, m_zValue.y);
}

void VtCamera::createViewMatrix()
{
  glm::vec3 rotation = VtEntity::getCopyRotation();

  glm::vec3 front = VtUtil::maths::getFront(rotation);
  glm::vec3 up    = VtUtil::maths::getUp(rotation);
  m_viewMatrix    = VtUtil::maths::createViewMatrix(VtEntity::getRefPosition(), front, up);
}
