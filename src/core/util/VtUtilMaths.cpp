//_Author : CrappyDevGuy, M12D09Y2020_//

#include "core/util/VtUtil.hpp"

glm::mat4 VtUtil::maths::createPerspectiveMatrix(float fov, float width, float height, float near, float far)
{
  glm::mat4 projection;

  float aspect = (float)width / (float)height;
  projection = glm::perspective(glm::radians(fov), aspect, near, far);
  projection[1][1] *= -1;

  return projection;
}

glm::mat4 VtUtil::maths::createOrthographicMatrix(float planeX, float planeY, float near, float far)
{
  return glm::ortho<float>((float)-planeX, (float)planeX, (float)-planeY, (float)planeY, (float)near, (float)far);
}


glm::mat4 VtUtil::maths::createViewMatrix(glm::vec3 position, glm::vec3 front,	glm::vec3 up)
{
  glm::mat4 translateMatrix = glm::mat4(1.0);
  translateMatrix = glm::lookAt(position, position + front, up);

  return translateMatrix;
}

glm::mat4 VtUtil::maths::createViewMatrix(glm::vec3 position, glm::vec3 rot)
{
  glm::vec3 front = VtUtil::maths::getFront(rot);
  glm::vec3 up    = VtUtil::maths::getUp(rot);
  
  glm::mat4 translateMatrix = glm::mat4(1.0);
  translateMatrix = glm::lookAt(position, position + front, up);

  return translateMatrix;
}

glm::mat4 VtUtil::maths::createTransformationMatrix(glm::vec3 translation, glm::vec3 rot, float sc)
{
  glm::mat4 trans = glm::mat4(1.0);
  trans = glm::translate(trans, translation);
  trans = glm::rotate(trans, glm::radians(rot.x), glm::vec3(1.0, 0.0, 0.0));
  trans = glm::rotate(trans, glm::radians(rot.y), glm::vec3(0.0, 1.0, 0.0));
  trans = glm::rotate(trans, glm::radians(rot.z), glm::vec3(0.0, 0.0, 1.0));
  trans = glm::scale(trans, glm::vec3(sc));
  return trans;
}

glm::mat4 VtUtil::maths::createTransformationMatrix(glm::vec3 translation, glm::vec3 rot, glm::vec3 sc)
{
  glm::mat4 trans = glm::mat4(1.0);
  trans = glm::translate(trans, translation);
  trans = glm::rotate(trans, glm::radians(rot.x), glm::vec3(1.0F, 0.0F, 0.0F));
  trans = glm::rotate(trans, glm::radians(rot.y), glm::vec3(0.0F, 1.0F, 0.0F));
  trans = glm::rotate(trans, glm::radians(rot.z), glm::vec3(0.0F, 0.0F, 1.0F));
  trans = glm::scale(trans, sc);
  return trans;
}

glm::vec3 VtUtil::maths::getFront(glm::vec3 const rotation)
{
	glm::vec3 r = glm::vec3(0.0);
	float cosY = glm::cos(glm::radians(rotation.y + 90.0f));
	float sinY = glm::sin(glm::radians(rotation.y + 90.0f));

	float cosP = glm::cos(glm::radians(rotation.x));
	float sinP = glm::sin(glm::radians(rotation.x));

	r.x = (cosY * cosP);
	r.y = sinP;
	r.z = (sinY * cosP);

	glm::normalize(r);
	return r;
}

glm::vec3 VtUtil::maths::getRight(glm::vec3 const rotation)
{
	glm::vec3 r = glm::vec3(0.0);
	float cosY = (float) glm::cos(glm::radians(rotation.y));
	float sinY = (float) glm::sin(glm::radians(rotation.y));
	r.x = cosY;
	r.z = sinY;
	return r;
}

glm::vec3 VtUtil::maths::getUp(glm::vec3 const rotation)
{
	glm::vec3 dir = VtUtil::maths::getFront(rotation);
	dir = glm::cross(dir, VtUtil::maths::getRight(rotation));
	return dir;
}

glm::vec3 VtUtil::maths::getLeft(glm::vec3 const rotation)
{
	return getRight(rotation) * -1.0F;
}

glm::vec2 VtUtil::maths::posOnCircle(glm::vec2 center, float radius, float time)
{
  float x = center.x + (radius*std::cos(time));
  float y = center.y + (radius*std::sin(time));
  return glm::vec2(x,y);
}
