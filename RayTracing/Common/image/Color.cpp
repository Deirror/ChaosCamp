#include "Color.h"

#include <random>
#include <algorithm>

#include "math/utils.h"

CRT_BEGIN

Color fromVec3ToColor(const Vec3& vec) {
	Color color;
	color.r = static_cast<unsigned char>(math::clamp(vec.x(), 0.f, 255.f));
	color.g = static_cast<unsigned char>(math::clamp(vec.y(), 0.f, 255.f));
	color.b = static_cast<unsigned char>(math::clamp(vec.z(), 0.f, 255.f));
	return color;
}

Vec3 fromColorToVec3(Color color) {
	return Vec3(color.r, color.g, color.b);
}

Color getRandomColor(unsigned char maxColorComponent) {
	unsigned char r = rand() % (maxColorComponent + 1);
	unsigned char g = rand() % (maxColorComponent + 1);
	unsigned char b = rand() % (maxColorComponent + 1);

	return crt::Color{ r, g, b };
}

CRT_END
