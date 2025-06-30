#include "Triangle.h"

CRT_BEGIN

Triangle::Triangle(Vec3 v0, Vec3 v1, Vec3 v2)
	: v0_(v0), v1_(v1), v2_(v2) {}

Vec3 Triangle::normal() const {
	return cross(v1_ - v0_, v2_ - v0_).normalized();
}

float Triangle::area() const {
	return (0.5f * cross(v1_ - v0_, v2_ - v0_).length());
}

float Triangle::area_squared() const {
	return 0.25f * cross(v1_ - v0_, v2_ - v0_).length_squared();
}

bool Triangle::isWindingAntiClockwise() const {
	return (cross(v1_ - v0_, v2_ - v0_).z() > -math::EPSILON_ZERO);
}

CRT_END
