#pragma once

#include "core/error.h"
#include "math/utils.h"

CRT_BEGIN

struct AspectHeight {
	AspectHeight() = default;
	AspectHeight(float ratio, float height)
		: ratio(ratio), height(height) { CRT_ENSURE(FLT_IS_POS(ratio) && FLT_IS_POS(height), "Negative number is passed"); }

	float width() const { return height * ratio; }

	static constexpr float kDefaultRatio = 16.f / 9.f;
	static constexpr float kDefaultHeight = 2.f;

	float ratio = kDefaultRatio;
	float height = kDefaultHeight;
};

CRT_END