#pragma once

#include "Defines.hpp"
#include "BasicMath.hpp"

namespace Poly
{

	using VectorIntType = int32_t;

	/// <summary>Class representing 2D integer vector.</summary>
	class ALIGN_16 CORE_DLLEXPORT Vector2i : public BaseObject<>{
	public:
		static const Vector2i ZERO;
		static const Vector2i UNIT_X;
		static const Vector2i UNIT_Y;

		/// <summary>Creates zero vector.</summary>
		constexpr Vector2i() : X(0), Y(0) {}

		/// <summary>Creates vector from int values.</summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		constexpr Vector2i(VectorIntType x, VectorIntType y) : X(x), Y(y) {}


		inline Vector2i(const Vector2i& rhs) : X(rhs.X), Y(rhs.Y) {}
		inline Vector2i& operator=(const Vector2i& rhs) { X = rhs.X; Y = rhs.Y; return *this; }

		/// <summary>Creates Vector2f based on this vector.</summary>
		/// <returns>Converted vector.</returns>
		class Vector2f ToVector2f() const;
		
		/// <summary>Negation operator</summary>
		inline Vector2i operator-() const { return Vector2i(-X, -Y); }

		// Comparisons
		bool operator==(const Vector2i& rhs) const;
		inline bool operator!=(const Vector2i& rhs) const { return !(*this == rhs); }

		// Operators with other vectors
		Vector2i operator+(const Vector2i& rhs) const;
		Vector2i operator-(const Vector2i& rhs) const;
		Vector2i& operator+=(const Vector2i& rhs);
		Vector2i& operator-=(const Vector2i& rhs);

		// Operators with ints
		Vector2i operator*(VectorIntType rhs) const;
		Vector2i operator/(VectorIntType rhs) const;
		Vector2i& operator*=(VectorIntType rhs);
		Vector2i& operator/=(VectorIntType rhs);

		CORE_DLLEXPORT friend std::ostream& operator<< (std::ostream& stream, const Vector2i& vec);

		// This structure allows to access vector elements by index or name.
		union
		{
			VectorIntType Data[2];
			struct { VectorIntType X, Y; };
		};

	private:
	};

}
