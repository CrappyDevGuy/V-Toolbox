//_Author : CrappyDevGuy, M01D18Y2021_//

#include "core/VtImporter.hpp"

#include "core/io/VtLogHandler.hpp"

#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

static void loadMaterial(const aiMaterial* pMaterial, VtImporter::Material::Data& t_data);
static void loadMesh(const aiScene* pScene, std::uint32_t index, const VtImporter::LoadInfo& t_loadInfo, VtImporter::FileData& t_data);
static void checkMesh(const aiMesh* pMesh, const VtImporter::LoadInfo& t_info);
static void loadTextures(const aiMaterial* pMaterial, VtImporter::Material::Data& t_data);

//_ Structures Functions _//

void VtImporter::Mesh::Data::merge(VtImporter::Mesh::Data t_data)
{
  vertices.insert(vertices.end(), t_data.vertices.begin(), t_data.vertices.end()); 
  normals.insert(normals.end(), t_data.normals.begin(), t_data.normals.end()); 
  textureCoords.insert(textureCoords.end(), t_data.textureCoords.begin(), t_data.textureCoords.end()); 

  indices.reserve(t_data.indices.size());
  std::uint32_t value = (vertices.size()/3) - (t_data.vertices.size()/3);
  for(unsigned int i = 0; i < t_data.indices.size(); i++)
    indices.push_back(t_data.indices[i]+value);
}

//_ Public Functions _//

void VtImporter::load(VtImporter::LoadInfo t_info, VtImporter::FileData& t_data)
{
  auto start = VtUtil::chrono::step();

  std::string fileName = t_info.name;
  if(fileName == "NULL")
    fileName = t_info.path;

  Assimp::Importer importer;
  const aiScene* pScene = importer.ReadFile(t_info.path, t_info.assimpflags);
  
  if(pScene != nullptr)
  	VtLogHandler::oStream("V-Toolbox", "VtImporter::load", "Success to read the file : " + fileName);
  else
  {
  	VtLogHandler::oStreamError("V-Toolbox", "VtImporter::load", "faild to read the file : " + fileName);
    std::string error = importer.GetErrorString();
  	VtLogHandler::oStreamFatalError("V-Toolbox", "VtImporter::load", "error : " + error);
  }

  //_BEGIN Data Loading_//

  auto meshCount = pScene->mNumMeshes;
  t_data.meshes.resize(meshCount);
  t_data.materials.resize(meshCount);

  VtLogHandler::oStream("V-Toolbox", "VtImporter::load", "Meshes Count : " + std::to_string(meshCount));

  for(auto i = 0u; i < meshCount; i++)
  {
    loadMesh(pScene, i, t_info, t_data);

    auto percent = ((i+1)*100)/meshCount;
    VtLogHandler::oStreamDebug("V-Toolbox", "VtImporter::load", "Loading Mesh : " + t_data.meshes[i].name + " | " + std::to_string(percent));
 }

  //_ END Data Loading_//
  auto end = VtUtil::chrono::step();
  double elapsed = VtUtil::chrono::elapsedTime<std::chrono::milliseconds>(start, end);
  std::string time = std::to_string(elapsed) + "ms";

  t_data.name = std::move(fileName);

  VtLogHandler::oStream("V-Toolbox", "VtImporter::load", "Success to load : " + fileName + " | " + time);
}

void VtImporter::mergeFileData(VtImporter::FileData& t_data, std::uint32_t srcID, std::uint32_t dstID)
{
  t_data.meshes[dstID].merge(t_data.meshes[srcID]);
}

void VtImporter::mergeAllFileData(VtImporter::FileData& t_data, std::uint32_t dstID)
{
  std::uint32_t size = t_data.meshes.size();
  for(auto i = 0u; i < size; i++)
  {
    if(i != dstID)
      VtImporter::mergeFileData(t_data, i, dstID);
  }
}

//_Static Functions_//

static void loadMesh(const aiScene* pScene, std::uint32_t index, const VtImporter::LoadInfo& t_loadInfo, VtImporter::FileData& t_data)
{
  const aiMesh* pMesh = pScene->mMeshes[index];

  checkMesh(pMesh, t_loadInfo);

  t_data.meshes[index].name = pMesh->mName.C_Str();

  std::uint32_t vertexCount = pMesh->mNumVertices; // maybe need to be multiply by 3

  if(pScene->mNumMaterials > 0){
    loadTextures(pScene->mMaterials[pMesh->mMaterialIndex], t_data.materials[index]);
    loadMaterial(pScene->mMaterials[pMesh->mMaterialIndex], t_data.materials[index]);
  }

  const aiVector3D* c_data = nullptr;
  for(std::uint32_t i = 0; i < vertexCount; i++)
  {
    c_data = &pMesh->mVertices[i];
    t_data.meshes[index].vertices.push_back(c_data->x);
    t_data.meshes[index].vertices.push_back(c_data->y);
    t_data.meshes[index].vertices.push_back(c_data->z);

    c_data = &pMesh->mNormals[i];
    t_data.meshes[index].normals.push_back(c_data->x);
    t_data.meshes[index].normals.push_back(c_data->y);
    t_data.meshes[index].normals.push_back(c_data->z);

    c_data = &pMesh->mTextureCoords[0][i];
    t_data.meshes[index].textureCoords.push_back(c_data->x);
    t_data.meshes[index].textureCoords.push_back(c_data->y);
  }

  t_data.meshes[index].indices.reserve(pMesh->mNumFaces);
  for(std::uint32_t i = 0; i < pMesh->mNumFaces; i++)
  {
    const aiFace& c_face = pMesh->mFaces[i];
    assert(c_face.mNumIndices == 3);

    t_data.meshes[index].indices.push_back(c_face.mIndices[0]);
    t_data.meshes[index].indices.push_back(c_face.mIndices[1]);
    t_data.meshes[index].indices.push_back(c_face.mIndices[2]);
  }
}

static void loadMaterial(const aiMaterial* pMaterial, VtImporter::Material::Data& t_data)
{
  aiColor3D color;
  pMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
  t_data.ambient = glm::vec3{color.r, color.g, color.b};

  pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
  t_data.diffuse = glm::vec3{color.r, color.g, color.b};

  pMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
  t_data.specular = glm::vec3{color.r, color.g, color.b};

  aiString name;
  pMaterial->Get(AI_MATKEY_NAME, name);
  t_data.name = name.C_Str();
}

static void loadTextures(const aiMaterial* pMaterial, VtImporter::Material::Data& t_data)
{
  for(auto i = 1u; i < 10; i++)
  {
    aiTextureType type = aiTextureType(i);
    aiString path;
    pMaterial->GetTexture(type, 0, &path, NULL, NULL, NULL, NULL, NULL);
    std::string strpath = path.C_Str();
    std::size_t t = strpath.find_last_of('/');
    strpath.erase(0, t+1);

    if(strpath != "")
      t_data.texturesName[i-1] = strpath;
  }
}

static void checkMesh(const aiMesh* pMesh, const VtImporter::LoadInfo& t_info)
{
  if(!VtUtil::file::exist(t_info.path))
    VtLogHandler::oStreamFatalError("V-Toolbox", "VtImporter::checkMesh", "File missing : " + t_info.path);
  else if(!pMesh->HasPositions())
    VtLogHandler::oStreamFatalError("V-Toolbox", "VtImporter::checkMesh", "No Vertex Data or Corrupt File");
  else if(!pMesh->HasFaces())
    VtLogHandler::oStreamFatalError("V-Toolbox", "VtImporter::checkMesh", "No Index Data or Corrupt File");
  else if(!pMesh->HasNormals())
    VtLogHandler::oStreamWarning("V-Toolbox", "VtImporter::checkMesh", "No Normals Data or Corrupt File");
  else if(!pMesh->HasTextureCoords(0))
    VtLogHandler::oStreamWarning("V-Toolbox", "VtImporter::checkMesh", "No Texture Coords Data or Corrupt File");
}
