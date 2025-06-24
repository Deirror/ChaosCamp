#include "Triangle.h"

CRT_BEGIN

Triangle::Triangle(Vec3 v0, Vec3 v1, Vec3 v2)
	: v0_(v0), v1_(v1), v2_(v2) {
	if (!isWindingAntiClockwise()) {
		std::swap(v1_, v2_);
	}
}

Vec3 Triangle::normal() const {
	Vec3 normal = cross(v1_ - v0_, v2_ - v0_);
	return normal.normalized();
}

float Triangle::area() const {
	Vec3 e1 = v1_ - v0_;
	Vec3 e2 = v2_ - v0_;
	return (0.5f * cross(e1, e2).length());
}

float Triangle::area_squared() const {
	Vec3 e1 = v1_ - v0_;
	Vec3 e2 = v2_ - v0_;
	return 0.25f * cross(e1, e2).length_squared();
}

bool Triangle::isWindingAntiClockwise() const {
	Vec3 e1 = v1_ - v0_;
	Vec3 e2 = v2_ - v0_;
	return (cross(e1, e2).z() > math::EPSILON_ZERO);
}

CRT_END
