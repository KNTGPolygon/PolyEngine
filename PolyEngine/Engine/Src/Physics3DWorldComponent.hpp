#pragma once

#include <map>

#include "ComponentBase.hpp"
#include "Physics3DSystem.hpp"

class btDiscreteDynamicsWorld;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btCollisionObject;

namespace Poly
{
	/// Config for physics simulation
	/// @see Physics3DWorldComponent
	/// @see Trigger3DComponent
	/// @see Rigidbody3DComponent
	struct ENGINE_DLLEXPORT Physics3DConfig
	{
	public:
		float TimeStep = 1.f / 60.f;
		Vector Gravity = Vector(0.f, -9.81f, 0.f);
		float TimeFactor = 1.f;
	};

	/// World component contains configuration for physics simulation and 
	/// many other important things per world but only thing the user has to be 
	/// concerned about is gravity.
	/// @see Physics3DConfig
	/// @see Trigger3DComponent
	/// @see Rigidbody3DComponent
	class ENGINE_DLLEXPORT Physics3DWorldComponent : public ComponentBase
	{
		friend void Physics3DSystem::Physics3DUpdatePhase(World* world);
		friend void Physics3DSystem::RegisterRigidbody(World* world, const UniqueID& entityID, EnumFlags<eCollisionGroup> collisionGroup, EnumFlags<eCollisionGroup> collidesWith);
		friend void Physics3DSystem::UnregisterRigidBody(World* world, const UniqueID& entityID);
		friend void Physics3DSystem::RegisterCollider(World* world, const UniqueID& entityID,	EnumFlags<eCollisionGroup> collisionGroup, EnumFlags<eCollisionGroup> collidesWith);
		friend void Physics3DSystem::UnregisterCollider(World* world, const UniqueID& entityID);
		friend bool Physics3DSystem::IsColliding(World* world, const UniqueID& firstID, const UniqueID& secondID);
		friend const ContactResult& Physics3DSystem::ContactPair(World* world, const UniqueID& firstID, const UniqueID& secondID);
		friend const ContactResult& Physics3DSystem::Contact(World* world, const UniqueID& entityID);
		friend const RaycastResult& Physics3DSystem::AllHitsRaycast(World* world, const Vector& from, const Vector& to);
		friend const RaycastResult& Physics3DSystem::ClosestHitRaycast(World* world, const Vector& from, const Vector& to);
	public:
		Physics3DWorldComponent(Physics3DConfig config);
		~Physics3DWorldComponent();

		void SetGravity(const Vector& g);
		void SetTimeStep(float timestep);
		void SetTimeFactor(float factor);

		const Vector& GetGravity() const { return Config.Gravity; }
		float GetTimeStep() const { return Config.TimeStep; }
		float GetTimeFactor() const { return Config.TimeFactor; }

	private:
		Physics3DConfig Config;

		std::unique_ptr<btDiscreteDynamicsWorld> DynamicsWorld;

		std::unique_ptr<btDefaultCollisionConfiguration> CollisionConfiguration;
		std::unique_ptr<btCollisionDispatcher> Dispatcher;
		std::unique_ptr<btBroadphaseInterface> Broadphase;
		std::unique_ptr<btSequentialImpulseConstraintSolver> Solver;

		float LastDeltaOverflow = 0.f;
		// raytest returns bullet rigidbody and we want to get UniqueID of that body
		std::map<const btCollisionObject*, UniqueID> BulletTriggerToEntity;
	};

	REGISTER_COMPONENT(WorldComponentsIDGroup, Physics3DWorldComponent)
} // namespace Poly