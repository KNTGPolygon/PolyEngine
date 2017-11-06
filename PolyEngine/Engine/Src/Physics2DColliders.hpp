#pragma once

#include "ComponentBase.hpp"

class b2PolygonShape;
class b2CircleShape;

namespace Poly
{
	class ENGINE_DLLEXPORT Box2DColliderComponent : public ComponentBase
	{
	public:
		Box2DColliderComponent(const Vector& size);
		~Box2DColliderComponent();

		void SetSize(const Vector& size);

		const b2PolygonShape* GetShape() const { return Shape.get(); }
	private:
		std::unique_ptr<b2PolygonShape> Shape;
	};

	REGISTER_COMPONENT(ComponentsFamily, Box2DColliderComponent)

	class ENGINE_DLLEXPORT Circle2DColliderComponent : public ComponentBase
	{
	public:
		Circle2DColliderComponent(float size);
		~Circle2DColliderComponent();

		void SetSize(float size);

		const b2CircleShape* GetShape() const { return Shape.get(); }
	private:
		std::unique_ptr<b2CircleShape> Shape;
	};

	REGISTER_COMPONENT(ComponentsFamily, Circle2DColliderComponent)
}