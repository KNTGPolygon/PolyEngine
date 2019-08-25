#pragma once

#include <pe/Defines.hpp>

#include <pe/core/storage/String.hpp>
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
		class ENGINE_DLLEXPORT SubMesh : public ::pe::core::BaseObject<>
		{
		public:
			struct ENGINE_DLLEXPORT Bone {
				Bone(core::storage::String name, core::math::Matrix boneFromModel) : name(name), boneFromModel(boneFromModel) {}

				core::storage::String name;
				core::math::Matrix boneFromModel;
			};

			SubMesh(const core::storage::String& path, aiMesh* mesh, aiMaterial* material);

			void LoadGeometry(aiMesh* mesh);
			void LoadBones(aiMesh* mesh);
			TextureResource* LoadTexture(const aiMaterial* material, const core::storage::String& path, const unsigned int aiType, const eTextureUsageType textureType);

			const Mesh& GetMeshData() const { return MeshData; }
			const IMeshDeviceProxy* GetMeshProxy() const { return MeshProxy.get(); }
			const core::math::AABox& GetAABox() const { return AxisAlignedBoundingBox; }
			std::vector<Bone> GetBones() const { return Bones; }
		private:
			std::vector<Bone> Bones;
			core::math::AABox AxisAlignedBoundingBox;
			Mesh MeshData;
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

				core::storage::String Name;
				std::vector<KeyValue<core::math::Vector>> Positions;
				std::vector<KeyValue<core::math::Quaternion>> Rotations;
				std::vector<KeyValue<core::math::Vector>> Scales;
			};

			struct ChannelLerpData
			{
				std::optional<Channel::KeyValue<core::math::Vector>> pos[2];
				std::optional<Channel::KeyValue<core::math::Vector>> scale[2];
				std::optional<Channel::KeyValue<core::math::Quaternion>> rot[2];
			};

			ChannelLerpData GetLerpData(core::storage::String channel, float time) const;

			core::storage::String Name;
			float Duration;
			float TicksPerSecond;
			std::map<core::storage::String, Channel> channels;
		};

		struct ENGINE_DLLEXPORT Bone {
			Bone(core::storage::String name) : name(name) {}

			core::storage::String name;
			core::math::Matrix prevBoneFromBone;
			core::math::Matrix boneFromModel;
			std::optional<size_t> parentBoneIdx = {};
			std::vector<size_t> childrenIdx;
		};

		MeshResource(const core::storage::String& path);
		virtual ~MeshResource();


		const std::vector<SubMesh*>& GetSubMeshes() const { return SubMeshes; }
		const Animation* GetAnimation(const core::storage::String& name) const
		{
			auto it = Animations.find(name);
			if (it == Animations.end())
				return nullptr;
			return it->second;
		}
		const core::math::AABox& GetAABox() const { return AxisAlignedBoundingBox; }
		const std::vector<Bone>& GetBones() const { return Bones; }

		const core::math::Matrix& GetModelFromSkeletonRoot() const { return ModelFromSkeletonRoot; }
	private:
		void LoadBones(aiNode* node);
		void PopulateBoneReferences(const std::map<core::storage::String, size_t>& nameToBoneIdx, aiNode* node, const core::math::Matrix& localTransform);

		core::math::Matrix ModelFromSkeletonRoot;
		std::vector<Bone> Bones;
		std::map<core::storage::String, Animation*> Animations;
		std::vector<SubMesh*> SubMeshes;
		core::math::AABox AxisAlignedBoundingBox;
	};
}