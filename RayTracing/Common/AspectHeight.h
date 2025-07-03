#pragma once

#include "crt.h"

#include "error.h"
#include "Math/utils.h"

CRT_BEGIN

struct AspectHeight {
	AspectHeight() = default;
	AspectHeight(float ratio, float height) 
		: ratio(ratio), height(height) { CRT_ENSURE(FLT_IS_POS(ratio), "Ratio is not positive"); CRT_ENSURE(FLT_IS_POS(height), "Height is not positive"); }

	float width() const { return static_cast<float>(height) * static_cast<float>(ratio); }

	float ratio, height;
};

CRT_END