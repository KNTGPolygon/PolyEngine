#pragma once

// STL
// This is only legal place for STL includes
#include <ostream>
#include <cmath>
#include <cassert>
#include <cstring>
#include <type_traits>
#include <limits>
#include <array>
#include <cstddef>
#include <memory>
#include <algorithm>
#include <iostream>
#include <string>

// stupid warning in MSVC about template specialization exporting, according to https://msdn.microsoft.com/en-US/library/esew7y1w.aspx it can be ignored
#if defined(_WIN32)
	#pragma warning(disable: 4251)
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ALIGN_16
#if defined(_WIN32)
	#define ALIGN_16 __declspec(align(16))
#elif defined(__GNUC__) || defined(__clang__)
	#define ALIGN_16 __attribute__ ((aligned(16)))
#else
	#define ALIGN_16 alignas(16)
#endif

#if defined(__GNUC__) && !defined(offsetof)
	#define offsetof(type, member) __builtin_offsetof(type, member)
#endif

// CORE_DLLEXPORT
#if defined(_WIN32)
	#if defined(_WINDLL) && defined(_CORE)
		#define CORE_DLLEXPORT __declspec(dllexport)
	#else
		#define CORE_DLLEXPORT __declspec(dllimport)
	#endif

	#if defined(_WINDLL) && defined(_ENGINE)
		#define ENGINE_DLLEXPORT __declspec(dllexport)
	#else
		#define ENGINE_DLLEXPORT __declspec(dllimport)
	#endif

	#if defined(_WINDLL) && defined(_GAME)
		#define GAME_DLLEXPORT __declspec(dllexport)
	#else
		#define GAME_DLLEXPORT __declspec(dllimport)
	#endif
#elif defined(__GNUC__) || defined(__clang__)
	#if defined(_DLL) && defined(_CORE)
		#define CORE_DLLEXPORT __attribute__ ((visibility ("default")))
	#else
		#define CORE_DLLEXPORT __attribute__ ((visibility ("default")))
	#endif

	#if defined(_DLL) && defined(_ENGINE)
		#define ENGINE_DLLEXPORT __attribute__ ((visibility ("default")))
	#else
		#define ENGINE_DLLEXPORT __attribute__ ((visibility ("default")))
	#endif

	#if defined(_DLL) && defined(_GAME)
		#define GAME_DLLEXPORT __attribute__ ((visibility ("default")))
	#else
		#define GAME_DLLEXPORT __attribute__ ((visibility ("default")))
	#endif
#else
	#define CORE_DLLEXPORT
	#define ENGINE_DLLEXPORT
	#define GAME_DLLEXPORT
#endif

#ifndef DISABLE_SIMD
#define DISABLE_SIMD false
#endif

// limits
constexpr auto MAX_FLOAT = std::numeric_limits<float>::max();
constexpr auto MIN_FLOAT = std::numeric_limits<float>::min();

// Assertions
#define ASSERTE(expr, msg) assert((expr) && #msg)
#define HEAVY_ASSERTE(expr, msg) assert((expr) && #msg)
#define STATIC_ASSERTE(expr, msg) static_assert(expr, msg)

// Utilities
#define BIT(x) (1u<<x)

#include "BaseObject.hpp"
