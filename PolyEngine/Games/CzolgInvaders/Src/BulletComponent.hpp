#pragma once

#include "ComponentBase.hpp"
#include <Timer.hpp>
#include <World.hpp>
#include <AABox.hpp>

namespace Poly {

	class GAME_DLLEXPORT BulletComponent : public ComponentBase
	{

	public:
		BulletComponent(float movementSpeed, const Vector& direction, const AABox& collision, double spawnTime);

		float GetMovementSpeed() const { return MovementSpeed; }
		const Vector& GetDirection() const { return Direction; }
		double GetLifeTme() const { return LifeTime; }
		double GetSpawnTme() const { return SpawnTime; }
		AABox& GetCollisionBox() { return CollisionBox; }

	private:
		double LifeTime = 4;
		double SpawnTime = 0.0f;
		float MovementSpeed = 100.0f;
		Vector Direction = { 0.0f, 0.0f, 0.0f };
		AABox CollisionBox;
	};
}