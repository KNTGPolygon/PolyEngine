#pragma once

#include "Defines.hpp"
#include "BasicMath.hpp"

namespace Poly {
	
	/// <summary>Class representing RGBA color.</summary>
	class CORE_DLLEXPORT Color : public BaseObject<>
	{
	public:
		Color();
		Color(float r, float g, float b, float a = 1.0f);

		// Operators with other Colors
		Color operator+(const Color& rhs) const;
		Color operator-(const Color& rhs) const;
		Color& operator+=(const Color& rhs);
		Color& operator-=(const Color& rhs);

		CORE_DLLEXPORT friend std::ostream& operator<< (std::ostream& stream, const Color& color);

		union {
			float Data[4];
			struct
			{
				float R, G, B, A;
			};
		};
	};
}