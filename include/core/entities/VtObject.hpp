//_Author : CrappyDevGuy, M01D18Y2021_//

#pragma once

#include "core/entities/VtEntity.hpp"

#include "core/util/VtUtil.hpp"

class VtObject : public VtEntity
{
  public:
    VtObject(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
    :VtEntity(position, rotation)
    ,m_scale{scale}
    {}

    VtObject(glm::vec3 position, glm::vec3 rotation)
    :VtEntity(position, rotation)
    {}

    VtObject(glm::vec3 position)
    :VtEntity(position)
    {}
    
    VtObject(const VtObject&) = delete;
    VtObject()=default;
    ~VtObject() {}

    VtObject& operator=(const VtObject&) = delete;
    VtObject& operator=(VtObject&& other) noexcept
    { 
      std::swap(other.m_transformationMatrix, m_transformationMatrix);
      std::swap(other.m_scale, m_scale);
      return *this;
    }

    VtObject(VtObject&& other) noexcept
    :m_transformationMatrix{std::move(other.m_transformationMatrix)}
    {}

    inline void createTransformationMatrix()
    {
      m_transformationMatrix = VtUtil::maths::createTransformationMatrix(VtEntity::getRefPosition(), VtEntity::getRefRotation(), m_scale);
    }

    inline auto& getRefTransformation()  noexcept { return m_transformationMatrix;            };
    inline auto  getCopyTransformation() noexcept { return glm::mat4{m_transformationMatrix}; };

    inline auto& getRefScale()  noexcept { return m_scale;            };
    inline auto  gefCopyScale() noexcept { return glm::vec3{m_scale}; };

    inline void  setScale(glm::vec3 scale) { m_scale = std::move(scale); };
    inline void  setScale(float scale)     { m_scale = glm::vec3{scale}; };

  private:
    glm::vec3 m_scale{1.0f};
    glm::mat4 m_transformationMatrix{0.0f};

};
