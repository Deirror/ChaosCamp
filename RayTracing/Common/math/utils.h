#pragma once

#include "core/crt.h"

CRT_BEGIN

namespace math {
	inline constexpr float EPSILON_RAY = 1e-4f;
	inline constexpr float EPSILON_ZERO = 1e-6f;

	inline constexpr float PI = 3.14159265358979323846f;
	inline constexpr float PI2 = 2.f * PI;
	inline constexpr float PI4 = 4.f * PI;
		
	inline constexpr float PI_RAD = PI / 180.f;
	inline constexpr float SLOPE_BIAS = 1e-3f;

	inline float radians(float degrees) {
		return degrees * PI_RAD;
	}

	inline float degrees(float radians) {
		return radians / PI_RAD;
	}

	inline float clamp(float value, float min, float max) {
		return (value < min) ? min : (value > max) ? max : value;
	}

	inline float bias(float dot) {
		return math::EPSILON_RAY + math::SLOPE_BIAS * (1.f - dot);
	}

	inline float randf() {
		return (float)rand() / RAND_MAX;
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

	#define FLT_IS_NEG(x) (x < -crt::math::EPSILON_ZERO)
	#define FLT_IS_POS(x) (x > crt::math::EPSILON_ZERO)
}

CRT_END

