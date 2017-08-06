#pragma once

#include "ComponentBase.hpp"
#include "Physics2DSystem.hpp"

class b2World;

namespace Poly
{
	struct ENGINE_DLLEXPORT Physics2DConfig
	{
	public:
		const float TimeStep = 1.0f / 60.0f;
		const size_t VelocityIterations = 8;
		const size_t PositionIterations = 3;
		const Vector Gravity = Vector(0.f, -9.81f, 0.f);
	};

	class Physics2DContactListener;

	class ENGINE_DLLEXPORT Physics2DWorldComponent : public ComponentBase
	{
		friend void Physics2DSystem::Physics2DUpdatePhase(World* world);
		friend class Physics2DContactListener;
		friend class RigidBody2DComponent;
	public:
		struct Collision
		{
			RigidBody2DComponent* rb;
			Vector Normal;
		};

		Physics2DWorldComponent(const Physics2DConfig& config);
		~Physics2DWorldComponent();

		const Dynarray<Collision>& GetCollidingBodies(RigidBody2DComponent* rb) const;

		void SetGravity(const Vector& gravity) const;
		const Vector& GetGravity() const { return Config.Gravity; }
	private:
		float LastDeltaOverflow = 0.f;

		const Physics2DConfig Config;
		std::unique_ptr<b2World> World;
		std::unique_ptr<Physics2DContactListener> ContactListener;

		std::unordered_map<RigidBody2DComponent*, Dynarray<Collision>> OverlapingBodies;
	};
}