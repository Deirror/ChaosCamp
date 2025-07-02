#pragma once

#include "crt.h"

CRT_BEGIN

struct AspectHeight {
	AspectHeight() = default;
	AspectHeight(float ratio, float height) : ratio(ratio), height(height) {}

	float width() const { return static_cast<float>(height) * static_cast<float>(ratio); }

	float ratio, height;
};

CRT_END