#include <EnginePCH.hpp>
#include "AnimationSystem.hpp"
#include <pe/api/ecs/Scene.hpp>

#include <Time/TimeSystem.hpp>
#include <Debugging/DebugDrawSystem.hpp>
#include <Rendering/MeshRenderingComponent.hpp>
#include <Rendering/Animation/BoneComponent.hpp>
#include <Rendering/Animation/SkeletalAnimationComponent.hpp>
#include <pe/api/ecs/DeferredTaskSystem.hpp>


using namespace Poly;

static const float MIN_TO_SEC = 60.f;

void AnimationSystem::OnUpdate(Scene* scene)
{
	for (auto&& [boneCmp] : scene->IterateComponents<BoneComponent>())
	{
		core::math::Vector parentPos = boneCmp->GetOwner()->GetParent()->GetTransform().GetGlobalTranslation();
		core::math::Vector myPos = boneCmp->GetOwner()->GetTransform().GetGlobalTranslation();
		DebugDrawSystem::DrawLine(scene, myPos, parentPos, core::math::Color::GREEN);
		DebugDrawSystem::DrawArrow(scene, myPos, boneCmp->GetOwner()->GetTransform().GetGlobalForward(), core::math::Color::RED);
	}

	// Iterate all skeletal meshes
	for (auto&& [animCmp, meshCmp] : scene->IterateComponents<SkeletalAnimationComponent, MeshRenderingComponent>())
	{
		if (animCmp->CheckFlags(eComponentBaseFlags::NEWLY_CREATED))
			CreateBoneStructure(animCmp, meshCmp);

		std::map<::pe::core::storage::String, std::vector<std::pair<::pe::core::math::Matrix, float>>> boneMatrices;
		std::vector<::pe::core::storage::String> animsToremove;

		for (auto& animKV : animCmp->ActiveAnimations)
		{
			// Store basic info
			const core::storage::String& animName = animKV.first;
			SkeletalAnimationState& animState = animKV.second;
			const MeshResource::Animation* anim = meshCmp->GetMesh()->GetAnimation(animName);

			if (!anim)
			{
				core::utils::gConsole.LogError("Animation with name {} not found!", animName);
				continue;
			}
			
			// Calculate correct dt
			float dt = (float)TimeSystem::GetTimerDeltaTime(scene, animState.Params.Timer) * animState.Params.PlaybackSpeed * MIN_TO_SEC;

			// Apply delay
			if (animState.DelayTime + dt < animState.Params.Delay)
			{
				// Still in delay, continue loop
				animState.DelayTime += dt;
				continue;
			}
			else if (animState.DelayTime < animState.Params.Delay)
			{
				// Reached delay end, correct the dt
				dt = animState.DelayTime + dt - animState.Params.Delay;
				animState.DelayTime = animState.Params.Delay;
			}

			// Set current time
			animState.Time = animState.Time + dt;

			// Check stop conditions
			animState.StopRequested = animState.StopRequested
				|| (!animState.Params.Loop && animState.Time > anim->Duration)
				|| (animState.Params.Loop && animState.Params.LoopCount.has_value() && animState.LoopCount >= animState.Params.LoopCount.value());

			// Correct time if stop condition occured or loop finished
			if (animState.Time >= anim->Duration)
			{
				// Animation loop finished, correct time value
				ASSERTE(anim->Duration > 0, "Invalid anim duration!");
				animState.LoopCount += (size_t)(animState.Time / anim->Duration);
				animState.Time = animState.StopRequested ? anim->Duration : fmodf(animState.Time, anim->Duration);
			}

			// Update bone positions, channel == bone
			for (auto& channel : anim->channels)
			{
				const core::storage::String& channelName = channel.first;
				auto lerpData = anim->GetLerpData(channelName, animState.Time);
				
				core::math::Vector pos = core::math::Vector::ZERO;
				core::math::Vector scale = core::math::Vector::ONE;
				core::math::Quaternion rot = core::math::Quaternion::IDENTITY;

				// Extract position value
				if (lerpData.pos[0].has_value())
				{
					auto posKey = lerpData.pos[0].value();
					pos = posKey.Value;
					if (lerpData.pos[1].has_value())
					{
						pos = core::math::Lerp(pos, lerpData.pos[1].value().Value, animState.Time - posKey.Time);
					}
				}
				
				// Extract scale value
				if (lerpData.scale[0].has_value())
				{
					auto scaleKey = lerpData.scale[0].value();
					scale = scaleKey.Value;
					if (lerpData.scale[1].has_value())
					{
						scale = core::math::Lerp(scale, lerpData.scale[1].value().Value, animState.Time - scaleKey.Time);
					}
				}

				// Extract rotation value
				if (lerpData.rot[0].has_value())
				{
					auto rotKey = lerpData.rot[0].value();
					rot = rotKey.Value;
					if (lerpData.rot[1].has_value())
					{
						const float t = std::min(animState.Time - rotKey.Time, 1.0f);
						rot = core::math::Quaternion::Slerp(rot, lerpData.rot[1].value().Value, t);
					}
				}

				// Push to the bone matrix blend list
				core::math::Matrix parentFromBone = core::math::Matrix::Compose(pos, rot, scale);
				boneMatrices[channelName].push_back({ parentFromBone, animState.Params.Weight });
			}

			// Prepare list of animation to remove
			if (animState.StopRequested && animState.Time >= anim->Duration)
				animsToremove.push_back(animName);
		}

		// Remove inactive animations
		for (const core::storage::String& animName : animsToremove)
			animCmp->ActiveAnimations.erase(animName);

		if (!boneMatrices.empty())
		{
			// Apply blended transformations to bone entities
			for (auto& bone : animCmp->Bones)
			{
				auto&& it = boneMatrices.find(bone->GetName());
				if(it != boneMatrices.end())
					bone->GetTransform().SetParentFromModel(::pe::core::math::Matrix::Blend(it->second));
			}

			// Populate ModelFromBone matrices.
			animCmp->ModelFromBone.clear();
			core::math::Matrix ModelFromWorld = animCmp->GetTransform().GetWorldFromModel().GetInversed();
			for (auto& bone : animCmp->Bones)
				animCmp->ModelFromBone[bone->GetName()] = ModelFromWorld * bone->GetTransform().GetWorldFromModel();
		}
	}
}

void Poly::AnimationSystem::StartAnimation(SkeletalAnimationComponent* cmp, const core::storage::String&  animationName, const SkeletalAnimationParams& params)
{
	auto&& it = cmp->ActiveAnimations.find(animationName);
	if (it != cmp->ActiveAnimations.end())
	{
		core::utils::gConsole.LogWarning("Starting animation [{}] when it's already running.");
	} 

	cmp->ActiveAnimations[animationName] = SkeletalAnimationState(params);
}

void Poly::AnimationSystem::StopAnimation(SkeletalAnimationComponent* cmp, const core::storage::String&  animationName, bool immediate)
{
	auto&& it = cmp->ActiveAnimations.find(animationName);
	if (it != cmp->ActiveAnimations.end())
	{
		core::utils::gConsole.LogError("Stopping animation [{}] when it's not running.");
	}
	else
	{
		it->second.StopRequested = true;
	}
}

bool Poly::AnimationSystem::IsAnimationActive(SkeletalAnimationComponent * cmp, const core::storage::String&  animationName)
{
	auto&& it = cmp->ActiveAnimations.find(animationName);
	return it != cmp->ActiveAnimations.end();
}

void Poly::AnimationSystem::OnComponentRemoved(SkeletalAnimationComponent* cmp)
{
	RemoveBoneStructure(cmp, cmp->GetSibling< MeshRenderingComponent>());
	core::utils::gConsole.LogDebug("SkeletalAnimationComponent removed successfully in AnimationSystem");
}

void Poly::AnimationSystem::CreateBoneStructure(SkeletalAnimationComponent* animCmp, MeshRenderingComponent* meshCmp)
{
	Entity* parent = animCmp->GetOwner();
	Scene* scene = parent->GetEntityScene();
	
	// Create skeleton
	Entity* skeleton = DeferredTaskSystem::SpawnEntityImmediate(scene);
	skeleton->SetParent(parent);
	skeleton->GetTransform().SetParentFromModel(meshCmp->GetMesh()->GetModelFromSkeletonRoot());
	skeleton->SetName("Mesh_Skeleton");

	// Create bones
	const auto& bones = meshCmp->GetMesh()->GetBones();
	for (const auto& bone : bones)
	{
		Entity* ent = DeferredTaskSystem::SpawnEntityImmediate(scene);
		ent->SetName(bone.name);
		DeferredTaskSystem::AddComponentImmediate<BoneComponent>(scene, ent);
		animCmp->Bones.push_back(ent);
	}

	// Initialize bone hierarchy
	for (size_t i = 0; i < bones.size(); ++i)
	{
		if (bones[i].parentBoneIdx.has_value())
			animCmp->Bones[i]->SetParent(animCmp->Bones[bones[i].parentBoneIdx.value()].Get());
		else
			animCmp->Bones[i]->SetParent(skeleton);
		animCmp->Bones[i]->GetTransform().SetParentFromModel(bones[i].prevBoneFromBone);
	}
}

void Poly::AnimationSystem::RemoveBoneStructure(SkeletalAnimationComponent * animCmp, MeshRenderingComponent * meshCmp)
{
}