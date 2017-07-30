#include "EnginePCH.hpp"

#include "Physics2DWorldComponent.hpp"
#include <Box2D/Box2D.h>

using namespace Poly;

namespace Poly {
	class Physics2DContactListener : public b2ContactListener
	{
	public:
		Physics2DContactListener(Physics2DWorldComponent* cmp)
			: Component(cmp)
		{}

		void BeginContact(b2Contact* contact)
		{
			RigidBody2DComponent* rb1 = static_cast<RigidBody2DComponent*>(contact->GetFixtureA()->GetUserData());
			RigidBody2DComponent* rb2 = static_cast<RigidBody2DComponent*>(contact->GetFixtureB()->GetUserData());
			b2WorldManifold manifold;
			contact->GetWorldManifold(&manifold);
			
			Vector normal(manifold.normal.x, manifold.normal.y, 0);
			Component->OverlapingBodies[rb1].PushBack(Physics2DWorldComponent::Collision{ rb2, normal });
			Component->OverlapingBodies[rb2].PushBack(Physics2DWorldComponent::Collision{ rb1, -normal });
		}

		void EndContact(b2Contact* contact)
		{
			RigidBody2DComponent* rb1 = static_cast<RigidBody2DComponent*>(contact->GetFixtureA()->GetUserData());
			RigidBody2DComponent* rb2 = static_cast<RigidBody2DComponent*>(contact->GetFixtureB()->GetUserData());

			for (int i = 0; i < Component->OverlapingBodies[rb1].GetSize(); ++i)
			{
				if (Component->OverlapingBodies[rb1][i].rb == rb2)
					Component->OverlapingBodies[rb1].RemoveByIdx(i);
			}
			
			for (int i = 0; i < Component->OverlapingBodies[rb2].GetSize(); ++i)
			{
				if (Component->OverlapingBodies[rb2][i].rb == rb1)
					Component->OverlapingBodies[rb2].RemoveByIdx(i);
			}
		}

		Physics2DWorldComponent* Component;
	};
}

Physics2DWorldComponent::Physics2DWorldComponent(const Physics2DConfig& config)
	: Config(config), World(std::make_unique<b2World>(b2Vec2(config.Gravity.X, config.Gravity.Y)))
{
	ContactListener = std::make_unique<Physics2DContactListener>(this);
	World->SetContactListener(ContactListener.get());
}

Poly::Physics2DWorldComponent::~Physics2DWorldComponent()
{
}

const Dynarray<Physics2DWorldComponent::Collision>& Poly::Physics2DWorldComponent::GetCollidingBodies(RigidBody2DComponent* rb) const
{
	static Dynarray<Physics2DWorldComponent::Collision> EMPTY;
	auto it = OverlapingBodies.find(rb);
	if (it == OverlapingBodies.end())
		return EMPTY;
	return it->second;
}

void Poly::Physics2DWorldComponent::SetGravity(const Vector& gravity) const
{
	World->SetGravity(b2Vec2(gravity.X, gravity.Y));
}
