#include "InvadersGame.hpp"

#include <CameraComponent.hpp>
#include <TransformComponent.hpp>
#include <MeshRenderingComponent.hpp>
#include <FreeFloatMovementComponent.hpp>
#include <Core.hpp>
#include <DeferredTaskSystem.hpp>
#include "ViewportWorldComponent.hpp"

using namespace Poly;

void InvadersGame::Init()
{
	Camera = DeferredTaskSystem::SpawnEntityImmediate(&Engine->GetWorld());
	DeferredTaskSystem::AddComponentImmediate<Poly::TransformComponent>(&Engine->GetWorld(), Camera);
	DeferredTaskSystem::AddComponentImmediate<Poly::CameraComponent>(&Engine->GetWorld(), Camera, 45.0f, 1.0f, 1000.f);
	DeferredTaskSystem::AddComponentImmediate<Poly::FreeFloatMovementComponent>(&Engine->GetWorld(), Camera, 10.0f, 0.003f);
	
	// Set some camera position
	Poly::TransformComponent* cameraTrans = Engine->GetWorld().GetComponent<Poly::TransformComponent>(Camera);
	cameraTrans->SetLocalTranslation(Vector(-23.1327f, 13.9473f, -25.7297f));
	cameraTrans->SetLocalRotation(Quaternion(EulerAngles{ 152.154_deg, 52.1159_deg, -180_deg }));
	
	/*for (int i = -2; i < 2; ++i)
	{
		for (int j = -2; j < 2; ++j)
		{
			auto ent = DeferredTaskSystem::SpawnEntityImmediate(&Engine->GetWorld());
			DeferredTaskSystem::AddComponentImmediate<Poly::TransformComponent>(&Engine->GetWorld(), ent);
			DeferredTaskSystem::AddComponent<Poly::MeshRenderingComponent>(&Engine->GetWorld(), ent, (const char*)"model-tank/tank.fbx");
			Poly::TransformComponent* entTransform = Engine->GetWorld().GetComponent<Poly::TransformComponent>(ent);
			entTransform->SetLocalTranslation(Vector(i * 2, 0, j * 5));
			GameEntities.PushBack(ent);
		}
	}*/

	Engine->GetWorld().GetWorldComponent<ViewportWorldComponent>()->SetCamera(0, Engine->GetWorld().GetComponent<Poly::CameraComponent>(Camera));
	Engine->RegisterUpdatePhase(GameMainSystem::GameUpdate, Poly::Engine::eUpdatePhaseOrder::UPDATE);
};

void InvadersGame::Deinit()
{
	DeferredTaskSystem::DestroyEntityImmediate(&Engine->GetWorld(), Camera);
	for(auto ent : GameEntities)
		DeferredTaskSystem::DestroyEntityImmediate(&Engine->GetWorld(), ent);
};

void GameMainSystem::GameUpdate(Poly::World* world)
{
	//test purpuoses, for DeferredTaskSystem
	static int pos = 0;
	static float deg = 0;
	if (pos < 400)
	{
		auto sth = DeferredTaskSystem::SpawnEntityImmediate(world);
		DeferredTaskSystem::AddComponentImmediate<Poly::TransformComponent>(world, sth);
		Poly::TransformComponent* entTransform = world->GetComponent<Poly::TransformComponent>(sth);
		entTransform->SetLocalTranslation(Vector(cos(deg)*10.0f, -10.0f + deg/20.0f, sin(deg)*10.0f));
		deg += 3.0f; pos++;
		DeferredTaskSystem::AddComponent<Poly::CameraComponent>(world, sth, 45.0f, 1.0f, 1000.f);
		DeferredTaskSystem::AddComponent<Poly::FreeFloatMovementComponent>(world, sth, 10.0f, 0.003f);
		DeferredTaskSystem::AddComponent<Poly::MeshRenderingComponent>(world, sth, (const char *)"model-tank/tank.fbx");//(const char*)"model-tank/tank.fbx");
		DeferredTaskSystem::RemoveComponent<Poly::FreeFloatMovementComponent>(world, sth);//just testing remove
		DeferredTaskSystem::RemoveComponent<Poly::CameraComponent>(world, sth);
		//DeferredTaskSystem::DestroyEntity(world, sth);
	}

	/*for (auto components : world->IterateComponents<Poly::MeshRenderingComponent, Poly::TransformComponent>())
	{
		auto transform = std::get<Poly::TransformComponent*>(components);
		if (transform)
		{
			transform->SetLocalRotation(Poly::Quaternion(Poly::Vector::UNIT_Y, Poly::Angle::FromDegrees(val * 300)));
		}
	}*/
}
