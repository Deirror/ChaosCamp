#pragma once

#include "crt.h"

CRT_BEGIN

namespace math {
	constexpr float EPSILON_RAY = 1e-4f;
	constexpr float EPSILON_ZERO = 1e-6f;

	constexpr float PI = 3.14159265358979323846f;

	inline float clamp(float value, float min, float max) {
		return (value < min) ? min : (value > max) ? max : value;
	}

	inline float to_radians(float degrees) {
		return degrees * (PI / 180.0f);
	}
}

CRT_END
