#pragma once

#include <Defines.hpp>
#include <Collections/Dynarray.hpp>
#include <Collections/String.hpp>
#include <Resources/ResourceBase.hpp>
#include <Resources/TextureResource.hpp>
#include <Resources/Mesh.hpp>
#include <Rendering/IRenderingDevice.hpp>

struct aiMesh;
struct aiMaterial;
struct aiAnimation;
struct aiNode;

typedef unsigned int GLuint;
 
namespace Poly
{
	class ENGINE_DLLEXPORT MeshResource : public ResourceBase
	{
		RTTI_DECLARE_TYPE_DERIVED(MeshResource, ResourceBase)
		{
			//RTTI_PROPERTY_AUTONAME(SubMeshes, RTTI::ePropertyFlag::NONE);
		}
	public:
		class ENGINE_DLLEXPORT SubMesh : public BaseObject<>
		{
		public:
			SubMesh(const String& path, aiMesh* mesh, aiNode* rootNode, aiMaterial* material);

			struct ENGINE_DLLEXPORT Bone {
				Bone(String name) : name(name) {}

				String name;
				Matrix boneFromModel;
				Matrix boneFromParentBone;
				Optional<size_t> parentBoneIdx;
				std::vector<size_t> childrenIdx;
			};

			void LoadGeometry(aiMesh* mesh);
			void LoadBones(aiMesh* mesh, aiNode* rootNode);
			TextureResource* LoadTexture(const aiMaterial* material, const String& path, const unsigned int aiType, const eTextureUsageType textureType);

			const Mesh& GetMeshData() const { return MeshData; }
			const IMeshDeviceProxy* GetMeshProxy() const { return MeshProxy.get(); }
			const AABox& GetAABox() const { return AxisAlignedBoundingBox; }
		private:
			void PopulateBoneReferences(const std::map<String, size_t>& nameToBoneIdx, aiNode* node, const Matrix& localTransform);


			AABox AxisAlignedBoundingBox;
			Mesh MeshData;
			Dynarray<Bone> Bones;
			std::unique_ptr<IMeshDeviceProxy> MeshProxy;
		};

		struct ENGINE_DLLEXPORT Animation {

			Animation(aiAnimation* anim);

			struct ENGINE_DLLEXPORT Channel {

				template<typename T>
				struct KeyValue
				{
					T Value;
					float Time;
				};
				String Name;
				Dynarray<KeyValue<Vector>> Positions;
				Dynarray<KeyValue<Quaternion>> Rotations;
				Dynarray<KeyValue<Vector>> Scales;
			};

			String Name;
			float Duration;
			float TicksPerSecond;
			Dynarray<Channel> channels;
		};

		MeshResource(const String& path);
		virtual ~MeshResource();


		const Dynarray<SubMesh*>& GetSubMeshes() const { return SubMeshes; }
		const Dynarray<Animation*>& GetAnimations() const { return Animations; }
		const Animation* GetAnimation(const String& name) const
		{
			for (Animation* anim : Animations)
			{
				if (anim->Name == name)
					return anim;
			}
			return nullptr;
		}
		const AABox& GetAABox() const { return AxisAlignedBoundingBox; }
	private:
		Dynarray<Animation*> Animations;
		Dynarray<SubMesh*> SubMeshes;
		AABox AxisAlignedBoundingBox;
	};
}