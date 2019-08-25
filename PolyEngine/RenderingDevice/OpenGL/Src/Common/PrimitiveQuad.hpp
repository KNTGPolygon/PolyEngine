#pragma once

#include <pe/Defines.hpp>
#include <Common/GLUtils.hpp>

namespace Poly
{
	struct PrimitiveQuad : public ::pe::core::BaseObject<>
	{
		PrimitiveQuad();
		~PrimitiveQuad();
	
		GLuint VAO = 0;
		GLuint VBO = 0;
	};
}