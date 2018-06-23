#include "Proxy/GLTextureDeviceProxy.hpp"
#include "Proxy/GLUtils.hpp"

using namespace Poly;

// static GLenum GetFormat(size_t channels) noexcept
// {
// 	switch (channels)
// 	{
// 	case 1:
// 		return GL_RED;
// 	case 3:
// 		return GL_RGB;
// 	case 4:
// 		return GL_RGBA;
// 	default:
// 		ASSERTE(false, "Invalid Channels!");
// 	}
// 	return 0;
// }

// static GLenum GetFormat(eTextureDataFormat format) noexcept
// {
// 	switch (format)
// 	{
// 		case Poly::eTextureDataFormat::RED:
// 			return GL_RED;
// 		case Poly::eTextureDataFormat::RGB:
// 			return GL_RGB;
// 		case Poly::eTextureDataFormat::RGBA:
// 			return GL_RGBA;
// 		default:
// 			ASSERTE(false, "Invalid format!");
// 	}
// 	return 0;
// }

// static GLenum GetGLInternalFormat(eTextureUsageType usage) noexcept
// {
// 	switch (usage)
// 	{
// 		case Poly::eTextureUsageType::ALBEDO:
// 		case Poly::eTextureUsageType::EMISSIVE:
// 		case Poly::eTextureUsageType::METALLIC:
// 		case Poly::eTextureUsageType::ROUGHNESS:
// 		case Poly::eTextureUsageType::AMBIENT_OCCLUSION:
// 			return GL_RGBA;
// 		case Poly::eTextureUsageType::NORMAL:
// 		case Poly::eTextureUsageType::HDR:
// 			return GL_RGB;
// 		case Poly::eTextureUsageType::FONT:
// 			return GL_RED;
// 		default:
// 			ASSERTE(false, "Invalid usage!");
// 	}
// 	return 0;
// }

GLTextureDeviceProxy::GLTextureDeviceProxy(size_t width, size_t height, eRenderTargetType internalUsage, GLuint internalFormat)
	: Width(width), Height(height),
	// InternalFormat(internalFormat), Format(GL_RGB),
	Usage(eTextureUsageType::RENDER_TARGET), RenderTargetType(internalUsage)
{
	InitTextureParams();
}

GLTextureDeviceProxy::GLTextureDeviceProxy(size_t width, size_t height, size_t channels, eTextureUsageType usage)
	: Width(width), Height(height), Channels(channels),
	// InternalFormat(GetGLInternalFormat(usage)), Format(GetFormat(channels)),
	Usage(usage), RenderTargetType(eRenderTargetType::NONE)
{
	InitTextureParams();
}

GLTextureDeviceProxy::~GLTextureDeviceProxy()
{
	if(TextureID > 0)
		glDeleteTextures(1, &TextureID);
}

void GLTextureDeviceProxy::SetContent(const unsigned char* data)
{
	ASSERTE(Usage != eTextureUsageType::HDR, "Invalid texture usage type, unsigned char* is for LDR image data");
	ASSERTE(Width > 0 && Height > 0, "Invalid arguments!");
	ASSERTE(TextureID > 0, "Texture is invalid!");
	ASSERTE(data, "Data pointer is nullptr!");

	glBindTexture(GL_TEXTURE_2D, TextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, (GLsizei)Width, (GLsizei)Height, 0, Format, GL_UNSIGNED_BYTE, data);

	if (Usage != eTextureUsageType::FONT)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	CHECK_GL_ERR();
}

void GLTextureDeviceProxy::SetContentHDR(const float* data)
{
	ASSERTE(Usage == eTextureUsageType::HDR, "Invalid texture usage type, float* is for HDR image data");
	ASSERTE(Width > 0 && Height > 0, "Invalid arguments!");
	ASSERTE(TextureID > 0 , "Texture is invalid!");
	ASSERTE(data, "Data pointer is nullptr!");

	glBindTexture(GL_TEXTURE_2D, TextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, (GLsizei)Width, (GLsizei)Height, 0, Format, GL_FLOAT, data);

	if (Usage != eTextureUsageType::FONT)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	CHECK_GL_ERR();
}

void GLTextureDeviceProxy::SetSubContent(size_t width, size_t height,
	size_t offsetX, size_t offsetY, const unsigned char* data)
{
	ASSERTE(Usage != eTextureUsageType::HDR, "Invalid texture usage type, unsigned char* is for LDR image data");
	ASSERTE(width + offsetX <= Width && height + offsetY <= Height && width > 0 && height > 0, "Invalid arguments!");
	ASSERTE(data, "Data pointer is nullptr!");

	glBindTexture(GL_TEXTURE_2D, TextureID);
	glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)offsetX, (GLint)offsetY, (GLsizei)width, (GLsizei)height, Format, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
	CHECK_GL_ERR();
}

void GLTextureDeviceProxy::Resize(const ScreenSize& size)
{
	Width = size.Width;
	Height = size.Height;

	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureID);

	if (RenderTargetType == eRenderTargetType::DEPTH_ATTACHEMENT)
		glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, (GLsizei)Width, (GLsizei)Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	if (RenderTargetType == eRenderTargetType::COLOR_ATTACHEMENT)
		glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, (GLsizei)Width, (GLsizei)Height, 0, GL_RGBA, GL_FLOAT, nullptr);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, (GLsizei)Width, (GLsizei)Height, 0, InternalFormat, GL_UNSIGNED_BYTE, nullptr);

	glBindTexture(GL_TEXTURE_2D, 0);
	CHECK_GL_ERR();
}

void GLTextureDeviceProxy::InitTextureParams()
{
	ASSERTE(Width > 0 && Height > 0, "Invalid arguments!");
	if (TextureID > 0)
	{
		return;
	}

	gConsole.LogInfo("GLTextureDeviceProxy::InitTextureParams usage: {}", (int)Usage);

	switch (Usage)
	{
		case eTextureUsageType::ALBEDO:
		case eTextureUsageType::EMISSIVE:
		case eTextureUsageType::METALLIC:
		case eTextureUsageType::ROUGHNESS:
		case eTextureUsageType::AMBIENT_OCCLUSION:
			InitTextureGamma();
			break;
		case eTextureUsageType::RENDER_TARGET:
			InitTextureLinear();
			break;
		case eTextureUsageType::NORMAL:
			InitTextureNormal();
			break;
		case eTextureUsageType::HDR:
			InitTextureHDR();
			break;
		case eTextureUsageType::FONT:
			InitTextureFont();
			break;
		default:
			ASSERTE(false, "Uknown eTextureUsageType!");
	}

	// else
	// {
	// 	glGenTextures(1, &TextureID);
	// 
	// 	if (TextureID <= 0)
	// 		throw RenderingDeviceProxyCreationFailedException();
	// 
	// 	if (Usage == eTextureUsageType::FONT)
	// 	{
	// 		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// 	}
	// 
	// 	glBindTexture(GL_TEXTURE_2D, TextureID);
	// 
	// 	if(InternalUsage == eInternalTextureUsageType::DEPTH_ATTACHEMENT)
	// 		glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, (GLsizei)Width, (GLsizei)Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	// 	if (InternalUsage == eInternalTextureUsageType::COLOR_ATTACHEMENT)
	// 		glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, (GLsizei)Width, (GLsizei)Height, 0, GL_RGBA, GL_FLOAT, nullptr);
	// 	else
	// 		glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, (GLsizei)Width, (GLsizei)Height, 0, InternalFormat, GL_FLOAT, nullptr);
	// 
	// 	if (InternalUsage == eInternalTextureUsageType::COLOR_ATTACHEMENT || InternalUsage == eInternalTextureUsageType::DEPTH_ATTACHEMENT)
	// 	{
	// 		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// 		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// 	}
	// 	else if (Usage == eTextureUsageType::FONT)
	// 	{
	// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// 	}
	// 	else
	// 	{
	// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 16);
	// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 	}
	// 
	// 	glBindTexture(GL_TEXTURE_2D, 0);
	// 	CHECK_GL_ERR();
	// }
}

void GLTextureDeviceProxy::InitTextureHDR()
{
	glGenTextures(1, &TextureID);

	if (TextureID <= 0)
		throw RenderingDeviceProxyCreationFailedException();

	glBindTexture(GL_TEXTURE_2D, TextureID);
	InternalFormat = GL_RGB16F;
	Format = GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, (GLsizei)Width, (GLsizei)Height, 0, GL_RGB, GL_FLOAT, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
	CHECK_GL_ERR();
}

void GLTextureDeviceProxy::InitTextureGamma()
{
	glGenTextures(1, &TextureID);

	if (TextureID <= 0)
		throw RenderingDeviceProxyCreationFailedException();

	glBindTexture(GL_TEXTURE_2D, TextureID);
	InternalFormat = Channels > 3 ? GL_SRGB8_ALPHA8 : GL_SRGB8;
	Format = Channels > 3 ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, (GLsizei)Width, (GLsizei)Height, 0, Format, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
	CHECK_GL_ERR();
}

void GLTextureDeviceProxy::InitTextureLinear()
{
	glGenTextures(1, &TextureID);

	if (TextureID <= 0)
		throw RenderingDeviceProxyCreationFailedException();

	glBindTexture(GL_TEXTURE_2D, TextureID);
	InternalFormat = Channels > 3 ? GL_RGBA8 : GL_RGB8;
	Format = Channels > 3 ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, (GLsizei)Width, (GLsizei)Height, 0, Format, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
	CHECK_GL_ERR();

	// RenderTarget code:
	// 	if(InternalUsage == eInternalTextureUsageType::DEPTH_ATTACHEMENT)
	// 		glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, (GLsizei)Width, (GLsizei)Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	// 	if (InternalUsage == eInternalTextureUsageType::COLOR_ATTACHEMENT)
	// 		glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, (GLsizei)Width, (GLsizei)Height, 0, GL_RGBA, GL_FLOAT, nullptr);
	// 	else
	// 		glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, (GLsizei)Width, (GLsizei)Height, 0, InternalFormat, GL_FLOAT, nullptr);
	// 
	// 	if (InternalUsage == eInternalTextureUsageType::COLOR_ATTACHEMENT || InternalUsage == eInternalTextureUsageType::DEPTH_ATTACHEMENT)
	// 	{
	// 		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// 		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// 	}

}

void GLTextureDeviceProxy::InitTextureNormal()
{
	glGenTextures(1, &TextureID);

	if (TextureID <= 0)
		throw RenderingDeviceProxyCreationFailedException();

	glBindTexture(GL_TEXTURE_2D, TextureID);
	InternalFormat = GL_RGB;
	Format = GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, (GLsizei)Width, (GLsizei)Height, 0, Format, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
	CHECK_GL_ERR();
}

void GLTextureDeviceProxy::InitTextureFont()
{
	glGenTextures(1, &TextureID);

	if (TextureID <= 0)
		throw RenderingDeviceProxyCreationFailedException();

	glBindTexture(GL_TEXTURE_2D, TextureID);
	InternalFormat = GL_R8;
	Format = GL_RED;
	glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, (GLsizei)Width, (GLsizei)Height, 0, Format, GL_UNSIGNED_BYTE, nullptr);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);
	CHECK_GL_ERR();
}