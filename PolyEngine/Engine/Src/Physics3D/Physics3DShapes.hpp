#pragma once

#include <pe/Defines.hpp>
#include <Resources/MeshResource.hpp>
#include <Physics3D/Physics3DSystem.hpp>
#include <Physics3D/Collider3DComponent.hpp>

namespace Poly
{
	struct Physics3DShapeImpl;
	struct Physics3DStaticMeshSourceImpl;

	enum class ePhysics3DShape
	{
		MESH,
		PLANE,
		BOX,
		SPHERE,
		CAPSULE,
		_COUNT
	};


	//------------------------------------------------------------------------------


	class ENGINE_DLLEXPORT Physics3DStaticMeshSource : public ::pe::core::BaseObject<>
	{
		friend class Physics3DStaticMeshShape;
	public:
		Physics3DStaticMeshSource();
		~Physics3DStaticMeshSource();
		void LoadMesh(const core::storage::String& meshPath, eResourceSource source);
		void LoadMesh(const MeshResource::SubMesh& subMesh);
		void AddTriangle(const core::math::Vector& a, const core::math::Vector& b, const core::math::Vector& c);

	private:
		std::unique_ptr<Physics3DStaticMeshSourceImpl> ImplData;
	};


	//------------------------------------------------------------------------------


	class ENGINE_DLLEXPORT Physics3DShape : public ::pe::core::BaseObject<>
	{
		friend void Physics3DSystem::EnsureInit(Scene* world, Entity* entity);
		friend core::math::Vector Physics3DSystem::CalculateInertia(const Physics3DShape* shape, float mass);
		friend void Collider3DComponent::SetShape(const Physics3DShape* shape);
	public:
		Physics3DShape(ePhysics3DShape type) : ShapeType(type) {}
		virtual ~Physics3DShape();
		const ePhysics3DShape ShapeType;

	protected:
		Physics3DShapeImpl* ImplData;
	};


	//------------------------------------------------------------------------------

	class ENGINE_DLLEXPORT Physics3DStaticMeshShape : public Physics3DShape
	{
	public:
		Physics3DStaticMeshShape(const Physics3DStaticMeshSource& source);
	};

	//------------------------------------------------------------------------------

	class ENGINE_DLLEXPORT Physics3DPlaneShape : public Physics3DShape
	{
	public:
		Physics3DPlaneShape(core::math::Vector normal, float halfExtent);
		const core::math::Vector Normal;
		const float HalfExtent;
	};

	//------------------------------------------------------------------------------

	class ENGINE_DLLEXPORT Physics3DBoxShape : public Physics3DShape
	{
	public:
		Physics3DBoxShape(core::math::Vector halfExtents);
		const core::math::Vector HalfExtents;
	};

	//------------------------------------------------------------------------------

	class ENGINE_DLLEXPORT Physics3DSphereShape : public Physics3DShape
	{
	public:
		Physics3DSphereShape(float radius);
		const float Radius;
	};

	//------------------------------------------------------------------------------

	class ENGINE_DLLEXPORT Physics3DCapsuleShape : public Physics3DShape
	{
	public:
		Physics3DCapsuleShape(float radius, float height);
		const float Radius;
		const float Height;
	};
} // namespace Poly