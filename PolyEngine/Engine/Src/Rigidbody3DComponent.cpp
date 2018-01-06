#include "EnginePCH.hpp"

#include "btBulletDynamicsCommon.h"
#include "Rigidbody3DComponent.hpp"

//********************************************************************************************************************************************
Poly::Rigidbody3DComponent::Rigidbody3DComponent(World* world, eRigidBody3DType type, Physics3DShape* shape,
	float restitution, float friction, float rollingFriction, float spinningFriction, float mass)
	: BodyWorld(world),
	Mass(mass),
	Restitution(restitution),
	Friction(friction),
	RollingFriction(rollingFriction),
	SpinningFriction(spinningFriction),
	Shape(shape),
	BodyType(type)
{
	switch (shape->ShapeType)
	{
	case ePhysics3DShape::PLANE:
	{
		Physics3DPlaneShape* plane = reinterpret_cast<Physics3DPlaneShape*>(shape);
		BulletShape = new btStaticPlaneShape(btVector3(plane->Normal.X, plane->Normal.Y, plane->Normal.Z), plane->HalfExtent);
	}
		break;

	case ePhysics3DShape::BOX:
	{
		Vector v = reinterpret_cast<Physics3DBoxShape*>(shape)->HalfExtents;
		BulletShape = new btBoxShape(btVector3(v.X, v.Y, v.Z));
	}
		break;

	case ePhysics3DShape::SPHERE:
		BulletShape = new btSphereShape(reinterpret_cast<Physics3DSphereShape*>(shape)->Radius);
		break;

	case ePhysics3DShape::CAPSULE:
	{
		Physics3DCapsuleShape* capsule = reinterpret_cast<Physics3DCapsuleShape*>(shape);
		BulletShape = new btCapsuleShape(capsule->Radius, capsule->Height);
	}
		break;	
	}
	
	BulletMotionState = new btDefaultMotionState();
	
	btVector3 inertia(0, 0, 0);

	switch (type)
	{
	case eRigidBody3DType::STATIC:
		break;

	case eRigidBody3DType::DYNAMIC:
		ASSERTE(type != eRigidBody3DType::STATIC && mass > 0, "Can't create dynamic body with 0 mass.");
		BulletShape->calculateLocalInertia(mass, inertia);
		break;
	}
	btRigidBody::btRigidBodyConstructionInfo CI(mass, BulletMotionState, BulletShape, inertia);
	CI.m_restitution = restitution;
	CI.m_friction = friction;
	CI.m_rollingFriction = rollingFriction;
	CI.m_spinningFriction = spinningFriction;
	BulletRigidBody = new btRigidBody(CI);
}

//********************************************************************************************************************************************
Poly::Rigidbody3DComponent::~Rigidbody3DComponent()
{
	Physics3DSystem::UnregisterRigidBody(BodyWorld, GetOwnerID());

	delete BulletRigidBody;
	delete BulletMotionState;
	delete BulletShape;
}

//********************************************************************************************************************************************
void Poly::Rigidbody3DComponent::ApplyForceToCenter(const Vector& force)
{
	BulletRigidBody->applyCentralForce(btVector3(force.X, force.Y, force.Z));
}

//********************************************************************************************************************************************
void Poly::Rigidbody3DComponent::ApplyImpulseToCenter(const Vector& impulse)
{
	BulletRigidBody->applyCentralImpulse(btVector3(impulse.X, impulse.Y, impulse.Z));
}

//********************************************************************************************************************************************
void Poly::Rigidbody3DComponent::ApplyDamping(float timestep)
{
	BulletRigidBody->applyDamping(timestep);
}

//********************************************************************************************************************************************
void Poly::Rigidbody3DComponent::UpdatePosition()
{
	TransformComponent* transCmp = GetSibling<TransformComponent>();
	ASSERTE(transCmp, "No transform on physics object!");
	ASSERTE(transCmp->GetParent() == nullptr, "Physics cannot be applied to child entity");
	
	Vector localTrans = transCmp->GetLocalTranslation();
	Quaternion localRot = transCmp->GetLocalRotation();
	
	btVector3 position(localTrans.X, localTrans.Y, localTrans.Z);
	btQuaternion orientation(localRot.X, localRot.Y, localRot.Z, localRot.W);
	
	btTransform initialTransform;
	initialTransform.setOrigin(position);
	initialTransform.setRotation(orientation);
	
	BulletRigidBody->setWorldTransform(initialTransform);
	BulletMotionState->setWorldTransform(initialTransform);
}

//********************************************************************************************************************************************
void Poly::Rigidbody3DComponent::SetLinearFactor(const Vector& factor)
{
	BulletRigidBody->setLinearFactor(btVector3(factor.X, factor.Y, factor.Z));
}

//********************************************************************************************************************************************
void Poly::Rigidbody3DComponent::ApplyGravity()
{
	BulletRigidBody->applyGravity();
}

//********************************************************************************************************************************************
void Poly::Rigidbody3DComponent::SetAngularFactor(float factor)
{
}

//********************************************************************************************************************************************
const Poly::Vector& Poly::Rigidbody3DComponent::GetAngularFactor()
{
	const btVector3& v = BulletRigidBody->getLinearVelocity();
	return Vector(v.x(), v.y(), v.z());
}

//********************************************************************************************************************************************
const Poly::Vector& Poly::Rigidbody3DComponent::GetAngularVelocity()
{
	const btVector3& v = BulletRigidBody->getAngularVelocity();
	return Vector(v.x(), v.y(), v.z());
}

//********************************************************************************************************************************************
void Poly::Rigidbody3DComponent::SetDamping(float linearDamping, float angularDamping)
{
	BulletRigidBody->setDamping(linearDamping, angularDamping);
}

//********************************************************************************************************************************************
float Poly::Rigidbody3DComponent::GetLinearDamping()
{
	return BulletRigidBody->getLinearDamping();
}

//********************************************************************************************************************************************
float Poly::Rigidbody3DComponent::GetAngularDamping()
{
	return BulletRigidBody->getAngularDamping();
}

//********************************************************************************************************************************************
void Poly::Rigidbody3DComponent::SetGravity(const Vector&  gravity)
{
	BulletRigidBody->setGravity(btVector3(gravity.X, gravity.Y, gravity.Z));
}

//********************************************************************************************************************************************
const Poly::Vector& Poly::Rigidbody3DComponent::GetGravity()
{
	const btVector3& g = BulletRigidBody->getGravity();
	return Vector(g.x(), g.y(), g.z());
}