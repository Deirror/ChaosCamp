#pragma once

#include "math/Vec3.h"

CRT_BEGIN

struct Color {
	static Color random(int maxColorComp);

	static constexpr int kMaxComp = 255;

	int r = 0, g = 0, b = 0;
};

Color fromVec3ToColor(const Vec3& vec);

CRT_END