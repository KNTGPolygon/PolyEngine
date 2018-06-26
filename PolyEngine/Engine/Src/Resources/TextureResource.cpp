#include "EnginePCH.hpp"

#include "Resources/TextureResource.hpp"
#include "Resources/ResourceManager.hpp"

using namespace Poly;

TextureResource::TextureResource(const String& path, eTextureUsageType usage)
{
	gConsole.LogInfo("TextureResource::TextureResource path: {} usage: {}", path, (int)usage);

	if (usage == eTextureUsageType::HDR) 
	{
		float* Image = LoadImageHDR(path.GetCStr(), &Width, &Height, &Channels);
	
		gConsole.LogInfo("TextureResource::TextureResource loaded width: {}, height: {}, channels: {}", Width, Height, Channels);

		TextureProxy = gEngine->GetRenderingDevice()->CreateTexture(Width, Height, Channels, usage);
		TextureProxy->SetContentHDR(Image);
		FreeImageHDR(Image);
	}
	else
	{
		unsigned char* Image = LoadImage(path.GetCStr(), &Width, &Height, &Channels);

		gConsole.LogInfo("TextureResource::TextureResource loaded width: {}, height: {}, channels: {}", Width, Height, Channels);

		TextureProxy = gEngine->GetRenderingDevice()->CreateTexture(Width, Height, Channels, usage);
		TextureProxy->SetContent(Image);
		FreeImage(Image);
	}
}

TextureResource::~TextureResource()
{
}
