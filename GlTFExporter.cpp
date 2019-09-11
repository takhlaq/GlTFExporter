// The MIT License (MIT)
//
// Copyright (c) 2019 Tahir Akhlaq
// contributors.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "GlTFExporter.h"

#ifndef TINYGLTF_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

//#define TINYGLTF_NO_STB_IMAGE_WRITE

#ifndef TINYGLTF_NO_STB_IMAGE_WRITE
#define STB_IMAGE_WRITE_IMPLEMENTATION
#endif
#endif

#include "tinygltf/tiny_gltf.h"

//using MaterialPtr = GlTFExporter::MaterialPtr;
//using MeshPtr = GlTFExporter::MeshPtr;
//using ModelPtr = GlTFExporter::ModelPtr;
//using TransformPtr = GlTFExporter::TransformPtr;
//using TransformedModelPtr = GlTFExporter::TransformedModelPtr;
//using GroupPtr = GlTFExporter::GroupPtr;

GlTFExporter::~GlTFExporter()
{
   //reset();
}

void GlTFExporter::reset()
{
   exportDir = "";
   if (!textureIdMap.empty())
      textureIdMap.clear();
   if (!!textureNameRefMap.empty())
      textureNameRefMap.clear();
   if (!materialIdMap.empty())
      materialIdMap.clear();
   if (!materialNameRefMap.empty())
      materialNameRefMap.clear();
   if (!modelIdMap.empty())
      modelIdMap.clear();
   if (!modelNameRefMap.empty())
      modelNameRefMap.clear();
   if (!groupIdMap.empty())
      groupIdMap.clear();
   if (!groupNameRefMap.empty())
      groupNameRefMap.clear();
   if (!meshIdMap.empty())
      meshIdMap.clear();
   if (!meshNameRefMap.empty())
      meshNameRefMap.clear();
   if (!nameCount.empty())
      nameCount.clear();
}

uint32_t GlTFExporter::addTransformedModelToGroup(TransformedModelPtr pModel, GroupPtr pGroup)
{
   pGroup->models.push_back(pModel);
   pModel->id = pGroup->models.size();
   return pModel->id;
}

uint32_t GlTFExporter::addMeshToModel(MeshPtr pMesh, ModelPtr pModel)
{
   auto it = meshNameRefMap.find(pMesh->name);

   if (it == meshNameRefMap.end())
   {
      pMesh->id = modelIdMap.size();
      meshNameRefMap.emplace(std::make_pair(pMesh->name, pMesh->id));
      meshIdMap[pMesh->id] = pMesh;
   }
   else
   {
      pMesh = meshIdMap[it->second];
   }
   //
   {
      for (auto& currMesh : pModel->meshes)
      {
         if (currMesh.second == pMesh)
         {
            return currMesh.first;
         }
      }
      pModel->meshes.emplace(pModel->meshes.size(), pMesh);
      //pMesh->modelIndex = pModel->meshes.size();
   }
   return pMesh->id;
}

GlTFExporter::MeshPtr GlTFExporter::getMesh(const std::string& name) const
{
   auto it = meshNameRefMap.find(name);

   if (it == meshNameRefMap.end())
   {
      return nullptr;
   }
   return meshIdMap.at(it->second);
}

// add model or return id
uint32_t GlTFExporter::addModel(ModelPtr pModel)
{
   auto it = modelNameRefMap.find(pModel->name);

   if (it == modelNameRefMap.end())
   {
      pModel->id = modelIdMap.size();
      modelNameRefMap.emplace(std::make_pair(pModel->name, pModel->id));
      modelIdMap[pModel->id] = pModel;
   }
   else
   {
      pModel = modelIdMap[it->second];
   }
   return pModel->id;
}

GlTFExporter::ModelPtr GlTFExporter::getModel(const std::string& name) const
{
   auto it = modelNameRefMap.find(name);
   if (it == modelNameRefMap.end())
      return nullptr;

   return modelIdMap.find(it->second)->second;
}

// add group or return id
uint32_t GlTFExporter::addGroup(GroupPtr pGroup)
{
   auto it = groupNameRefMap.find(pGroup->name);

   assert(it == groupNameRefMap.end());
   {
      pGroup->id = groupNameRefMap.size();
      groupNameRefMap.emplace(std::make_pair(pGroup->name, pGroup->id));
      groupIdMap[pGroup->id] = pGroup;
   }
   return pGroup->id;
}

GlTFExporter::GroupPtr GlTFExporter::getGroup(const std::string& name) const
{
   auto it = groupNameRefMap.find(name);
   if (it == groupNameRefMap.end())
      return nullptr;
   return groupIdMap.at(it->second);
}

// add material or get id
uint32_t GlTFExporter::addMaterialToMesh(MaterialPtr pMaterial, MeshPtr pMesh)
{
   auto it = materialNameRefMap.find(pMaterial->name);
   if (it == materialNameRefMap.end())
   {
      pMaterial->name = pMesh->name;
      pMaterial->id = materialNameRefMap.size();
      materialNameRefMap.emplace(std::make_pair(pMaterial->name, pMaterial->id));
      materialIdMap[pMaterial->id] = pMaterial;
   }
   else
   {
      pMaterial = materialIdMap[it->second];
   }
   pMesh->materialRef = pMaterial->id;
   return pMaterial->id;
}

GlTFExporter::MaterialPtr GlTFExporter::getMaterial(const std::string& name)
{
   auto it = materialNameRefMap.find(name);
   if (it == materialNameRefMap.end())
      return nullptr;

   return materialIdMap[it->second];
}

uint32_t GlTFExporter::addTextureToMaterial(TexturePtr pTexture, MaterialPtr pMaterial, TextureSlot slot)
{
   auto it = textureNameRefMap.find(pMaterial->name);
   if (it == textureNameRefMap.end())
   {
      pTexture->id = textureNameRefMap.size();
      textureNameRefMap.emplace(std::make_pair(pTexture->name, pTexture->id));
      textureIdMap[pTexture->id] = pTexture;
   }
   else
   {
      pTexture = textureIdMap[it->second];
   }
   pMaterial->textureMap[slot].push_back(pTexture);
   return pTexture->id;
}

GlTFExporter::TexturePtr GlTFExporter::getTexture(const std::string& name)
{
   auto it = textureNameRefMap.find(name);
   if (it == textureNameRefMap.end())
      return nullptr;

   return textureIdMap[it->second];
}

void GlTFExporter::exportAllMaterials()
{
   for (auto& matPair : materialIdMap)
   {
      auto pMaterial = matPair.second;
   }
}

std::vector<double> vec3ToDoubleVec(const GlTFExporter::CVector3& v)
{
   std::vector<double> ret;
   ret.push_back(v.x); ret.push_back(v.y); ret.push_back(v.z);
   return ret;
}

std::vector<double> vec4ToDoubleVec(const GlTFExporter::CVector4& v)
{
   std::vector<double> ret;
   ret.push_back(v.x); ret.push_back(v.y); ret.push_back(v.z); ret.push_back(v.w);
   return ret;
}

void GlTFExporter::exportGlTFTexture(TexturePtr pTexture, tinygltf::Material* pGlMat, tinygltf::TextureInfo* pGlTexInfo, GlTFModelPtr pGlModel)
{
   if (pTexture->glTextureId == -1)
   {
      pTexture->glTextureId = pGlModel->textures.size();

      std::string texFilePath(exportDir + pTexture->filePath);
      std::ofstream of(texFilePath, std::ios::trunc | std::ios::binary);
      of.write((const char*)pTexture->data.data(), pTexture->data.size());
      of.close();


      tinygltf::Texture glTex;
      glTex.source = pGlModel->images.size();
      glTex.name = pTexture->name;

      pGlModel->textures.push_back(glTex);

      tinygltf::Image glImg;
      glImg.width = pTexture->width;
      glImg.height = pTexture->height;
      glImg.uri = pTexture->filePath;
      
      pGlModel->images.push_back(glImg);
   }
}

void GlTFExporter::exportGlTFMaterial(MaterialPtr pMaterial, GlTFModelPtr pGlModel)
{
   static uint32_t materialId = 0, textureId = 0;

   tinygltf::Material glMat;
   glMat.name = pMaterial->name;
   glMat.doubleSided = true;
   
   std::ofstream file(exportDir + "MISSING_TEXTURES_" + pMaterial->name + ".txt", std::ios::trunc);
   file.close();
   file.open(exportDir + "MISSING_TEXTURES_" + pMaterial->name + ".txt", std::ios::app);

   for (const auto& slotTexPair : pMaterial->textureMap)
   {
      tinygltf::TextureInfo glTexInfo;
      int slotIndex = 0;
      int texCoordIndex = 0;

      auto slotType = slotTexPair.first;

      for (auto pTex : slotTexPair.second)
      {
         if (pTex)
         {
            exportGlTFTexture(pTex, &glMat, &glTexInfo, pGlModel);
            auto& glTex = pGlModel->textures.at(pTex->glTextureId);

            glTexInfo.index = pTex->glTextureId;
            glTexInfo.texCoord = texCoordIndex;

            // todo: this is fucked
            // just dumping whatever i can for now
            switch (slotType)
            {
               case TextureSlot::Normal:
               {
                  tinygltf::NormalTextureInfo glNTexInfo;
                  glNTexInfo.index = glTexInfo.index;
                  glNTexInfo.texCoord = glTexInfo.texCoord;
                  glMat.normalTexture = glNTexInfo;
               }
               break;
               case TextureSlot::Occlusion:
               {
                  tinygltf::OcclusionTextureInfo glOTexInfo;
                  glOTexInfo.index = glTexInfo.index;
                  glOTexInfo.texCoord = glTexInfo.texCoord;
                  glMat.occlusionTexture = glOTexInfo;
               }
               break;
               case TextureSlot::Emissive:
               {
                  glMat.emissiveTexture = glTexInfo;
               }
               break;
               case TextureSlot::Albedo:
               {
                  glMat.pbrMetallicRoughness.baseColorTexture = glTexInfo;
               }
               break;
               case TextureSlot::Specular:
               {
                  glMat.pbrMetallicRoughness.metallicRoughnessTexture = glTexInfo;
               }
               default:
               {
                  std::string debugStr("MATERIAL: " + pMaterial->name + " ID: " + std::to_string(pGlModel->materials.size()) + "\n");
                  debugStr += "TEXTURE: " + pTex->name + " ID: " + std::to_string(pTex->glTextureId) + "\n";
                  file.write(debugStr.c_str(), debugStr.size());
                     //glMat.extensions["UNKNOWN_TEXTURE_SLOT_" + pTex->name].Keys().push_back(pTex->name);
               }
               break;
            }
         }
         //texCoordIndex++;
      }
   }
   file.close();
   pGlModel->materials.push_back(glMat);
}

void GlTFExporter::exportGlTFModel(ModelPtr pModel, GlTFModelPtr pGlModel)
{
   auto exportBinPath = exportDir + pModel->name + ".bin";;

   static uint32_t bufferId = 0, bufferViewId = 0, meshId = 0;
   bufferId = pGlModel->nodes.size();
   bufferViewId = pGlModel->bufferViews.size();

   pModel->glBufferId = bufferId;

   auto& tform = pModel->transformation;
   auto& tl = tform.translation;
   auto& rot = tform.rotation;
   auto& scl = tform.scale;

   //std::ofstream modelBin(exportBinPath, std::ios::trunc);
   //modelBin.close();
   //modelBin.open(exportBinPath, std::ios::app | std::ios::binary);

   uint32_t bufferOffset = 0;
   CVector3 min(999999, 999999, 999999), max(-999999, -999999, -999999);

   tinygltf::Buffer buf;
   buf.name = pModel->name;
   buf.uri = pModel->name + ".bin";

   // gltf things
   tinygltf::Node glNode;
   tinygltf::Mesh glMesh;

   glNode.name = pModel->name + "_" + std::to_string(nameCount[pModel->name]++);
   glNode.translation = vec3ToDoubleVec(pModel->transformation.translation);
   glNode.rotation = vec4ToDoubleVec(pModel->transformation.rotation);
   glNode.scale = vec3ToDoubleVec(pModel->transformation.scale);

   glMesh.name = glNode.name;

   for (const auto& meshPair : pModel->meshes)
   {
      auto pMesh = meshPair.second;

      tinygltf::Primitive glPrim;
      glPrim.mode = 4;

      // calc min max
      for (const auto v : pMesh->vertices)
      {
         min = min.Min(v);
         max = max.Max(v);
      }

      {
         auto pMaterial = materialIdMap[pMesh->materialRef];
         uint32_t oldMaterialIndex = pGlModel->materials.size();
         exportGlTFMaterial(pMaterial, pGlModel);
         if (oldMaterialIndex != pGlModel->materials.size())
            glPrim.material = pGlModel->materials.size() - 1;
      }

      // buffers
      static uint32_t indicesStride = sizeof(uint32_t);
      static uint32_t vertsStride = sizeof(CVector3);
      static uint32_t uvStride = sizeof(CVector2);
      static uint32_t normStride = sizeof(CVector3);
      static uint32_t colorsStride = sizeof(CVector4);

      uint32_t indicesLen = pMesh->indices.size() * indicesStride; // count * sizeof(uint32_t)
      uint32_t vertsLen = pMesh->vertices.size() * vertsStride; // count * sizeof(float) * (x, y, z)
      uint32_t uv1Len = pMesh->uv1.size() * uvStride; // count * sizeof(float) * (x, y)
      uint32_t uv2Len = pMesh->uv2.size() * uvStride; // count * sizeof(float) * (x, y)
      uint32_t normLen = pMesh->normals.size() * normStride; // count * sizeof(float) * (x ,y, z)
      uint32_t colorsLen = pMesh->colors.size() * colorsStride; // count * sizeof(float) * (x, y, z, w)

      auto pushBvAccPair = [&](const std::string& name, uint32_t stride, uint32_t numElements, char* pData, uint32_t len) -> std::pair< tinygltf::BufferView*, tinygltf::Accessor* >
      {
         uint32_t length = len;

         tinygltf::BufferView glBv;
         tinygltf::Accessor glAcc;

         glBv.buffer = bufferId;
         glBv.byteLength = length;
         glBv.byteOffset = bufferOffset;
         glBv.name = pMesh->name + "_" + name + "_VIEW";

         glAcc.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
         glAcc.type = TINYGLTF_TYPE_VEC3;
         glAcc.normalized = false;

         if (name == "INDICES")
         {
            glAcc.type = TINYGLTF_TYPE_SCALAR;
            glAcc.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT;

            glPrim.indices = pGlModel->accessors.size();
         }
         if (name.find("TEXCOORD") != -1)
         {
            glAcc.type = TINYGLTF_TYPE_VEC2;
         }
         else if (name.find("COLOR") != -1)
         {
            glAcc.type = TINYGLTF_TYPE_VEC4;
         }
         else if (name == "POSITION")
         {
            glAcc.minValues = vec3ToDoubleVec(min);
            glAcc.maxValues = vec3ToDoubleVec(max);
         }


         glAcc.name = pMesh->name + "_" + name + "_ACCESSOR";
         glAcc.bufferView = bufferViewId;
         glAcc.byteOffset = 0;
         glAcc.count = numElements;


         if (name != "INDICES")
         {
            glPrim.attributes[name] = bufferViewId;
         }

         buf.data.resize(buf.data.size() + length);
         memcpy(buf.data.data() + bufferOffset, pData, length);
         //modelBin.write(pData, length);

         bufferViewId++;
         bufferOffset += length;

         pGlModel->accessors.push_back(glAcc);
         pGlModel->bufferViews.push_back(glBv);

         bufferViewId = pGlModel->bufferViews.size();

         return std::make_pair(&pGlModel->bufferViews.at(pGlModel->bufferViews.size() - 1), &pGlModel->accessors.at(pGlModel->accessors.size() - 1));
      };


      {
         auto pair = pushBvAccPair("INDICES", indicesStride, pMesh->indices.size(), (char*)pMesh->indices.data(), indicesLen);
      }
      {
         auto pair = pushBvAccPair("POSITION", vertsStride, pMesh->vertices.size(), (char*)pMesh->vertices.data(), vertsLen);
      }
      {
         auto pair = pushBvAccPair("TEXCOORD_0", uvStride, pMesh->uv1.size(), (char*)pMesh->uv1.data(), uv1Len);
      }
      {
         auto pair = pushBvAccPair("TEXCOORD_1", uvStride, pMesh->uv2.size(), (char*)pMesh->uv2.data(), uv2Len);
      }
      {
         auto pair = pushBvAccPair("NORMAL", normStride, pMesh->normals.size(), (char*)pMesh->normals.data(), normLen);
      }
      {
         auto pair = pushBvAccPair("COLOR_0", colorsStride, pMesh->colors.size(), (char*)pMesh->colors.data(), colorsLen);
      }

      glMesh.primitives.push_back(glPrim);

   }
   pGlModel->meshes.push_back(glMesh);
   pGlModel->nodes.push_back(glNode);
   pGlModel->buffers.push_back(buf);
   //modelBin.close();
}

void GlTFExporter::exportGlTFTransformedModel(TransformedModelPtr pTransformedModel, GlTFModelPtr pGlModel, tinygltf::Node* pParentNode, tinygltf::Scene* pScene)
{
   static uint32_t childNodeId = 0;
   auto& tform = pTransformedModel->transformation;

   tinygltf::Node glTNode;
   glTNode.translation = vec3ToDoubleVec(tform.translation);
   glTNode.rotation = vec4ToDoubleVec(tform.rotation);
   glTNode.scale = vec3ToDoubleVec(tform.scale);

   // get the actual model's node and add this as a child
   {
      auto pModel = modelIdMap[pTransformedModel->modelRef];
      auto modelNode = pGlModel->nodes[pModel->glBufferId];
      glTNode.mesh = pModel->glBufferId;
      
      modelNode.children.push_back(pGlModel->nodes.size());
      glTNode.name = pModel->name + "_" + std::to_string(nameCount[pModel->name]++);;

   }

   if (pScene)
      pScene->nodes.push_back(pGlModel->nodes.size());
   if (pParentNode)
      pParentNode->children.push_back(pGlModel->nodes.size());
   
   pGlModel->nodes.push_back(glTNode);
}

void GlTFExporter::exportGlTFGroup(GroupPtr pGroup, GlTFModelPtr pGlModel, tinygltf::Node* pParentNode, tinygltf::Scene* pScene)
{
   auto& tform = pGroup->transformation;

   std::shared_ptr<tinygltf::Node> glGroupNodePtr = std::make_shared<tinygltf::Node>();

   tinygltf::Node& glGroupNode = *glGroupNodePtr.get();
   glGroupNode.name = pGroup->name + "_" + std::to_string(nameCount[pGroup->name]++);
   glGroupNode.translation = vec3ToDoubleVec(tform.translation);
   glGroupNode.rotation = vec4ToDoubleVec(tform.rotation);
   glGroupNode.scale = vec3ToDoubleVec(tform.scale);

   for (const auto& pModel : pGroup->models)
      exportGlTFTransformedModel(pModel, pGlModel, &glGroupNode, nullptr);
   
   for (const auto& pSubGroup : pGroup->subGroups)
      exportGlTFGroup(pSubGroup, pGlModel, &glGroupNode, pScene);

   if (pParentNode)
      pParentNode->children.push_back(pGlModel->nodes.size());

   // only add group nodes to the scene
   if (pGroup->subGroups.size() > 0)
      pScene->nodes.push_back(pGlModel->nodes.size());
   
   pGlModel->nodes.push_back(glGroupNode);
}

void GlTFExporter::doExport(const std::string& fileName, const std::string& exportDirectory)
{
   tinygltf::Scene scene;
   scene.name = fileName;

   exportDir = exportDirectory;
   if (exportDir == "")
      exportDir = "./";

   if (exportDir[exportDir.size() - 1] != '/' && exportDir[exportDir.size() - 1] != '\\')
      exportDir += "/";

   if (!std::experimental::filesystem::exists(exportDir))
      std::experimental::filesystem::create_directories(exportDir);
      
   if (scene.name == "")
      scene.name = "out.gltf";

   std::shared_ptr<tinygltf::Model> pGlModel = std::make_shared<tinygltf::Model>();

   tinygltf::Asset asset;

   // export models first since we'll point to these later
   for (auto& model : modelIdMap)
   {
      exportGlTFModel(model.second, pGlModel);
   }

   for (auto& group : groupIdMap)
   {
      tinygltf::Node rootGroupNode;
      exportGlTFGroup(group.second, pGlModel, &rootGroupNode, &scene);
      pGlModel->nodes.push_back(rootGroupNode);
   }

   asset.generator = "takhlaq/GlTFExporter";
   asset.version = "2.0";
   asset.copyright = "lmao";

   pGlModel->asset = asset;
   pGlModel->scenes.push_back(scene);


   std::string jsonFileName = fileName;

   tinygltf::TinyGLTF loader;
   loader.WriteGltfSceneToFile(pGlModel.get(), exportDir + jsonFileName, false, true, true, false);
}
