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

#ifndef _GLTF_EXPORTER_H
#define _GLTF_EXPORTER_H

#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdio>

#include <memory>
#include <set>

#include <filesystem>

#define _GLTF_EXPORTER_STANDALONE

namespace tinygltf
{
   struct Mesh;
   class Model;
   class Node;
   struct Scene;
   struct Material;
   struct TextureInfo;
};

using GlTFMeshPtr = std::shared_ptr<tinygltf::Mesh>;
using GlTFModelPtr = std::shared_ptr<tinygltf::Model>;
using GlTFNodePtr = std::shared_ptr<tinygltf::Node>;
using GlTFScenePtr = std::shared_ptr<tinygltf::Scene>;

class GlTFExporter;

class GlTFExporter
{
public:
   enum ErrorEnum : int
   {
      InvalidId = -1
   };

   enum class TextureSlot
   {
      Albedo,
      Specular,
      Normal,
      Emissive,
      Occlusion,
      Ambient,
      MetallicRoughness,
   };
   
   enum class TextureFormat
   {
      DXT1,
      DXT3,
      DXT5,
      ARGB,
      PNG,
      BMP
   };

   class Texture;
   class Material;
   class Mesh;
   class Model;
   class Transform;
   class TransformedModel;
   class Group;

   using TexturePtr = std::shared_ptr<Texture>;
   using MaterialPtr = std::shared_ptr<Material>;
   using MeshPtr = std::shared_ptr<Mesh>;
   using ModelPtr = std::shared_ptr<Model>;
   using TransformPtr = std::shared_ptr<Transform>;
   using TransformedModelPtr = std::shared_ptr<TransformedModel>;
   using GroupPtr = std::shared_ptr<Group>;


   #ifdef _GLTF_EXPORTER_STANDALONE
   class CVector2
   {
   public:
      float x{ 0 }, y{ 0 };
      CVector2(float x, float y){ this->x = x; this->y = y; }
      CVector2() {}
      ~CVector2() {}
   };
   class CVector3
   {
   public:
      float x{ 0 }, y{ 0 }, z{ 0 };
      CVector3(float x, float y, float z){ this->x = x; this->y = y; this->z = z; }
      CVector3() {}
      ~CVector3() {}

      CVector3 Min(const CVector3& rhs) const
      {
         return CVector3(std::min(x, rhs.x), std::min(y, rhs.y), std::min(z, rhs.z));
      }

      CVector3 Max(const CVector3& rhs) const
      {
         return CVector3(std::max(x, rhs.x), std::max(y, rhs.y), std::max(z, rhs.z));
      }
   };
   class CVector4
   {
   public:
      float x{ 0 }, y{ 0 }, z{ 0 }, w{ 0 };
      CVector4(float x, float y, float z, float w){ this->x = x; this->y = y; this->z = z; this->w = w; }
      CVector4() {}
      ~CVector4() {}
   };
   #endif

   class Vec2Double
   {
   public:
      double x{ 0 }, y{ 0 };
      Vec2Double() {}
      Vec2Double(CVector2& v) { x = v.x; y = v.y; }
      ~Vec2Double() {}
   };
   
   class Vec3Double
   {
   public:
      double x{ 0 }, y{ 0 }, z{ 0 };
      Vec3Double() {}
      Vec3Double(CVector3& v) { x = v.x; y = v.y; y = v.y; }
      ~Vec3Double() {}
   };

   class Vec4Double
   {
   public:
      double x{ 0 }, y{ 0 }, z{ 0 }, w{ 0 };
      Vec4Double() {}
      Vec4Double(CVector4& v) { x = v.x; y = v.y; z = v.z; w = v.w; }
      ~Vec4Double() {}
   };

   class Texture : public std::enable_shared_from_this<Texture>
   {
   public:
      int32_t id{ ErrorEnum::InvalidId };
      uint32_t width{ 0 };
      uint32_t height{ 0 };

      int32_t glTextureId{ ErrorEnum::InvalidId };

      TextureFormat format; // DXT1, DXT5, A8R8G8B8, X8R8G8B8, etc..

      std::string name;
      std::string filePath;
      std::vector<uint8_t> data;

      Texture(){}
      ~Texture() {}
   };

   class Material : public std::enable_shared_from_this<Material>
   {
   public:
      int32_t id{ ErrorEnum::InvalidId };
      std::string name;
      std::string filePath;
      
      std::map<TextureSlot, std::vector<TexturePtr>> textureMap; // <slot, <textures>>

      Material() {}
      ~Material() {}
   };

   class Mesh : public std::enable_shared_from_this<Mesh>
   {
   public:
      int32_t id{ ErrorEnum::InvalidId };
      int32_t materialRef{ ErrorEnum::InvalidId };
      std::string name;
      std::vector<CVector3> vertices;
      std::vector<CVector2> uv1, uv2, uv3, uv4;
      std::vector<CVector4> colors;
      std::vector<CVector3> normals;
      std::vector<uint32_t> indices;

      Mesh() {}
      ~Mesh() {}

      bool Mesh::operator== (const Mesh& rhs) const
      {
         return id == rhs.id || name == rhs.name;
      }
   };

   class Transform : public std::enable_shared_from_this<Transform>
   {
   public:
      CVector3 translation = CVector3(0, 0, 0);
      CVector4 rotation = CVector4(0, 0, 0, 1);
      CVector3 scale = CVector3(1, 1, 1);
      
      Transform(){}
      Transform(CVector3 translation, CVector4 rotation, CVector3 scale)
      {
         this->translation = translation;
         this->rotation = rotation;
         this->scale = scale;
      }
      ~Transform() {}
   };

   class Model : public std::enable_shared_from_this<Model>
   {
   public:
      int32_t id{ ErrorEnum::InvalidId };
      std::string name;

      int32_t glBufferId{ ErrorEnum::InvalidId };
      std::vector<uint32_t> glMeshIds;
      
      std::map<int, MeshPtr> meshes;
      Transform transformation;
      Model() {}
      ~Model() {}
   };

   class TransformedModel : public std::enable_shared_from_this<TransformedModel>
   {
   public:
      int32_t id{ ErrorEnum::InvalidId };
      int32_t modelRef{ ErrorEnum::InvalidId };
      Transform transformation;
   };

   class Group : public std::enable_shared_from_this<Group>
   {
   public:
      int32_t id{ ErrorEnum::InvalidId };
      int32_t type{ ErrorEnum::InvalidId };
      std::string name;
      Transform transformation;
      std::vector<TransformedModelPtr> models;
      std::vector<GroupPtr> subGroups;

      Group() {}
      ~Group() {}

      // todo: this is fuckin stupid, fix it legit
      // adds a subgroup to group, copies models if this group already exists here
      void addGroup(GroupPtr pGroup)
      {
         for (auto pSubGroup : subGroups)
            if (pSubGroup->name == pGroup->name)
            {
               for (auto pModel : pGroup->models)
                  pSubGroup->models.push_back(pModel);

               pGroup->models.clear();
               return;
            }
         subGroups.push_back(pGroup);
      }
   };
   
   std::map<std::string, int32_t> nameCount;

   std::map<std::string, int32_t> textureNameRefMap;
   std::map<std::string, int32_t> materialNameRefMap;
   std::map<std::string, int32_t> meshNameRefMap;
   std::map<std::string, int32_t> modelNameRefMap;
   std::map<std::string, int32_t> groupNameRefMap;

   std::map<int32_t, TexturePtr> textureIdMap;
   std::map<int32_t, MaterialPtr> materialIdMap;
   std::map<int32_t, MeshPtr> meshIdMap;
   std::map<int32_t, ModelPtr> modelIdMap;
   std::map<int32_t, GroupPtr> groupIdMap;

   std::string exportDir;

   void reset();
   int32_t addTransformedModelToGroup(TransformedModelPtr pModel, GroupPtr pGroup);
   
   int32_t addMeshToModel(MeshPtr pMesh, ModelPtr pModel);
   MeshPtr getMesh(const std::string& name) const;
   
   int32_t addModel(ModelPtr pModel);
   ModelPtr getModel(const std::string& name) const;
   
   int32_t addGroup(GroupPtr pGroup);
   GroupPtr getGroup(const std::string& name) const;

   int32_t addMaterialToMesh(MaterialPtr pMaterial, MeshPtr pMesh);
   MaterialPtr getMaterial(const std::string& name);

   int32_t addTextureToMaterial(TexturePtr pTexture, MaterialPtr pMaterial, TextureSlot slot);
   TexturePtr getTexture(const std::string& name);

   void exportAllMaterials();


   void GlTFExporter::exportGlTFTexture(TexturePtr pTexture, tinygltf::Material* pGlMat, tinygltf::TextureInfo* pGlTexInfo, GlTFModelPtr pGlModel);
   void exportGlTFMaterial(MaterialPtr pMaterial, GlTFModelPtr pGlModel);
   void exportGlTFModel(ModelPtr pModel, GlTFModelPtr pGlModel);
   void exportGlTFTransformedModel(TransformedModelPtr pTransformedModel, GlTFModelPtr pGlModel, tinygltf::Node* pParentNode, tinygltf::Scene* pScene);
   void exportGlTFGroup(GroupPtr pGroup, GlTFModelPtr pGlModel, tinygltf::Node* pParentNode, tinygltf::Scene* pScene);

   void doExport(const std::string& fileName, const std::string& exportDirectory, bool prettyPrint = true);



   GlTFExporter() = default;
   ~GlTFExporter(){}
   GlTFExporter(const GlTFExporter&) = delete;
   GlTFExporter& operator=(const GlTFExporter&) = delete;
};

#endif