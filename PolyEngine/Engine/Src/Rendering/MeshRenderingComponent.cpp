#include <EnginePCH.hpp>

#include <Rendering/Camera/CameraComponent.hpp>
#include <Rendering/MeshRenderingComponent.hpp>
#include <Resources/ResourceManager.hpp>

using namespace Poly;

RTTI_DEFINE_COMPONENT(::Poly::MeshRenderingComponent)

MeshRenderingComponent::MeshRenderingComponent(const core::storage::String& meshPath, eResourceSource source)
{
	Mesh = ResourceManager<MeshResource>::Load(meshPath, source);
	
	if (Mesh)
	{
		size_t materialsNum = GetMesh()->GetSubMeshes().size();
		Materials.resize(materialsNum);
		for (size_t i = 0; i < materialsNum; ++i)
		{
			SetMaterial(i, Material());
		}
	}
}

Poly::MeshRenderingComponent::~MeshRenderingComponent()
{
	if (Mesh)
		ResourceManager<MeshResource>::Release(Mesh);
}

std::optional<::pe::core::math::AABox> Poly::MeshRenderingComponent::GetBoundingBox(eEntityBoundingChannel channel) const
{
	if (channel != eEntityBoundingChannel::RENDERING || !Mesh)
		return {};
	return Mesh->GetAABox();
}
