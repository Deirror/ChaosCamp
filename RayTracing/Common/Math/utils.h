#pragma once

#include "crt.h"

CRT_BEGIN

namespace math {
	constexpr float EPSILON_RAY = 1e-4f;
	constexpr float EPSILON_ZERO = 1e-6f;

	constexpr float ZERO = 0.0f;

	constexpr float PI = 3.14159265358979323846f;

	inline float to_radians(float degrees) {
		return degrees * (PI / 180.0f);
	}

	inline float to_degrees(float radians) {
		return radians / (PI / 180.0f);
	}

	inline float clamp(float value, float min, float max) {
		return (value < min) ? min : (value > max) ? max : value;
	}

	inline float max(float a, float b) {
		return (a > b) ? a : b;
	}

	inline float min(float a, float b) {
		return (a < b) ? a : b;
	}

	inline float abs(float value) {
		return (value < 0.0f) ? -value : value;
	}

	#define FLT_IS_ZERO(x) (abs(x) < crt::math::EPSILON_ZERO)
	#define FLT_NOT_ZERO(x) (abs(x) > crt::math::EPSILON_ZERO)

	#define FLT_IS_NEG(x) (x < -crt::math::EPSILON_ZERO)
	#define FLT_IS_POS(x) (x > crt::math::EPSILON_ZERO)

	#define INT_IS_NEG(x) (x < NULL)
	#define INT_IS_POS(x) (x > NULL)
}

CRT_END

