#include "Color.h"

#include <random>
#include <algorithm>

#include "math/utils.h"

CRT_BEGIN

Color fromVec3ToColor(const Vec3& vec) {
	int r = static_cast<int>(math::clamp(vec.x(), 0.f, 255.f));
	int g = static_cast<int>(math::clamp(vec.y(), 0.f, 255.f));
	int b = static_cast<int>(math::clamp(vec.z(), 0.f, 255.f));

	return Color{ r, g, b };
}

Color Color::random(int maxColorComp) {
	int r = rand() % (maxColorComp + 1);
	int g = rand() % (maxColorComp + 1);
	int b = rand() % (maxColorComp + 1);

	return Color{ r, g, b };
}

CRT_END
