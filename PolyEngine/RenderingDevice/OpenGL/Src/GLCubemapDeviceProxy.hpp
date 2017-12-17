#pragma once

#include <IRenderingDevice.hpp>
#include "GLUtils.hpp"

namespace Poly
{
	class GLCubemapDeviceProxy : public ICubemapDeviceProxy
	{
	public:
		GLCubemapDeviceProxy(size_t width, size_t height);
		virtual ~GLCubemapDeviceProxy();
		
		void SetContent(unsigned int side, const unsigned char* data) override;

		GLuint GetTextureID() const { return TextureID; }

	private:
		void InitCubemapParams();

		size_t Width = 0;
		size_t Height = 0;
		GLuint TextureID = 0;

		friend class GLRenderingDevice;
	};
}