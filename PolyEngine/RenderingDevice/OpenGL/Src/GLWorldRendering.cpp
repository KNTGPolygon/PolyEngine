#include "GLRenderingDevice.hpp"

#include <ECS/World.hpp>
#include <Configs/DebugConfig.hpp>
#include <Rendering/Viewport.hpp>

#include <Rendering/Camera/CameraComponent.hpp>
#include <Rendering/MeshRenderingComponent.hpp>
#include <UI/ScreenSpaceTextComponent.hpp>
#include <Rendering/ViewportWorldComponent.hpp>
#include <Rendering/PostprocessSettingsComponent.hpp>

#include "Proxy/GLTextFieldBufferDeviceProxy.hpp"
#include "Proxy/GLTextureDeviceProxy.hpp"
#include "Pipeline/RenderingPassBase.hpp"
#include "Proxy/GLMeshDeviceProxy.hpp"
#include "Common/GLUtils.hpp"

#include "ForwardRenderer.hpp"
#include "TiledForwardRenderer.hpp"

using namespace Poly;

//------------------------------------------------------------------------------

void GLRenderingDevice::Init()
{
	gConsole.LogInfo("GLRenderingDevice::Init");

	GetExtensions();

	Renderer = CreateRenderer();
	Renderer->Init();

	InitPrograms();
}

IRendererInterface* GLRenderingDevice::CreateRenderer()
{
	IRendererInterface* renderer = nullptr;

	switch (RendererType)
	{
		case GLRenderingDevice::eRendererType::FORWARD:
			renderer = new ForwardRenderer(this);
			break;

		case GLRenderingDevice::eRendererType::TILED_FORWARD:
			renderer = new TiledForwardRenderer(this);
			break;

		default:
			ASSERTE(false, "Uknown eRenderingModeType");
	}

	return renderer;
}

void GLRenderingDevice::RenderWorld(World* world)
{
	const ScreenSize screenSize = gEngine->GetRenderingDevice()->GetScreenSize();

// 	glDepthMask(GL_TRUE);
// 	glEnable(GL_DEPTH_TEST);

// Clear FBO's
// 	for (eGeometryRenderPassType type : IterateEnum<eGeometryRenderPassType>())
// 		if (GeometryRenderingPasses[type])
// 			GeometryRenderingPasses[type]->ClearFBO();
// 
// 	for (ePostprocessRenderPassType type : IterateEnum<ePostprocessRenderPassType>())
// 		if (PostprocessRenderingPasses[type])
// 			PostprocessRenderingPasses[type]->ClearFBO();

	// For each visible viewport draw it
	for (auto& kv : world->GetWorldComponent<ViewportWorldComponent>()->GetViewports())
	{
		// Set viewport rect (TOOO change it to propper rect, not box)
		CameraComponent* cameraCmp = kv.second.GetCamera();
		const AARect& rect = kv.second.GetRect();

		glViewport((int)(rect.GetMin().X * screenSize.Width), (int)(rect.GetMin().Y * screenSize.Height),
			(int)(rect.GetSize().X * screenSize.Width), (int)(rect.GetSize().Y * screenSize.Height));

		Renderer->Render(world, rect, cameraCmp);
	}

	// Signal frame end
	EndFrame();
}