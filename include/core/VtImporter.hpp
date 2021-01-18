//_Author : CrappyDevGuy, M01D18Y2021_//

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <unordered_map>
#include <vector>
#include <string>

#include <assimp/postprocess.h>

#include "core/util/VtUtil.hpp"

namespace VtImporter
{      
  namespace Textures
  {
    constexpr static const std::uint32_t 
    Diffuse      = 0,
    Specular     = 1,
    Ambient      = 2,
    Emissive     = 3,
    Height       = 4,
    Normals      = 5,
    Shininess    = 6,
    Opacity      = 7,
    Displacement = 8,
    LightMap     = 9,
    Reflection   = 10;
  }

  namespace Material
  {
    struct Data
    {
      std::string name = "NULL";
      glm::vec3 ambient, diffuse, specular;
      std::unordered_map<std::uint32_t, std::string> texturesName;
    };
  }

  namespace Animation
  {
    struct BoneTransform
    {
      glm::vec3 position;
      glm::quat rotation;
    };

    struct KeyFrame
    {
      std::vector<VtImporter::Animation::BoneTransform> bonesTransform;
      float timestamp;
    };

    struct Data
    {
      std::vector<VtImporter::Animation::KeyFrame> keyframes;
      std::vector<std::uint32_t> boneIDs;
      std::vector<float> weights;
    };
  }

  namespace Mesh
  {
    struct Data
    {
      std::string name = "NULL";

      std::vector<float> vertices,
                         normals,
                         textureCoords;

      std::vector<std::uint32_t> indices;

      bool hasNormals()
      { return !normals.empty(); };

      bool hasTextureCoords()
      { return !textureCoords.empty(); };

      void merge(VtImporter::Mesh::Data t_data);

    };
  }

  struct FileData
  {
    std::string name = "NULL";
    std::vector<VtImporter::Material::Data> materials;
    std::vector<VtImporter::Animation::Data> animations;
    std::vector<VtImporter::Mesh::Data> meshes;
  };

  struct LoadInfo
  {
    std::string path = "NULL",
                name = "NULL"; // just for log

    int assimpflags;

  };

  void load(VtImporter::LoadInfo t_info, VtImporter::FileData& t_data);

  void mergeFileData(VtImporter::FileData& t_data, std::uint32_t srcID, std::uint32_t dstID);
  void mergeAllFileData(VtImporter::FileData& t_data, std::uint32_t dstID);
}
