#pragma once

#include "crt.h"

CRT_BEGIN

namespace math {
	constexpr float EPSILON_RAY = 1e-4f;
	constexpr float EPSILON_ZERO = 1e-6f;

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

	#define FLT_IS_ZERO(x) (std::abs(x) < crt::math::EPSILON_ZERO)
	#define FLT_NOT_ZERO(x) (std::abs(x) > crt::math::EPSILON_ZERO)

	#define FLT_IS_NEG(x) (x < -crt::math::EPSILON_ZERO)
	#define FLT_IS_POS(x) (x > crt::math::EPSILON_ZERO)

	#define INT_IS_NEG(x) (x < NULL)
	#define INT_IS_POS(x) (x > NULL)
}

CRT_END

