#pragma once

#include "core/error.h"
#include "math/utils.h"

CRT_BEGIN

struct AspectHeight {
	AspectHeight() = default;
	AspectHeight(float ratio, float height) 
		: ratio(ratio), height(height) { CRT_ENSURE(FLT_IS_POS(ratio), "Ratio is not positive"); CRT_ENSURE(FLT_IS_POS(height), "Height is not positive"); }

	float width() const { return static_cast<float>(height) * static_cast<float>(ratio); }

	static constexpr float DEFAULT_RATIO = 16.f / 9.f;
	static constexpr float DEFAULT_HEIGHT = 2.f;

	float ratio = DEFAULT_RATIO, height = DEFAULT_HEIGHT;
};

CRT_END