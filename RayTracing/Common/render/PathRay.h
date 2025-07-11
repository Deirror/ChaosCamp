#pragma once

#include "math/Ray.h"

CRT_BEGIN

class PathRay : public Ray {
public:
	PathRay(const Vec3& origin, const Vec3& direction, float ior = 1.f) : Ray(origin, direction), ior_(ior) {}
	PathRay(const Ray& ray, float ior = 1.f) : Ray(ray), ior_(ior) {}

	short depth() const { return depth_; }
	void depth(short depth) { depth_ = depth; }

	PathRay& operator++() { ++depth_; return *this; }
	PathRay operator++(int dummy) { PathRay temp = *this; ++depth_; return temp; }

	PathRay& operator--() { --depth_; return *this; }
	PathRay operator--(int dummy) { PathRay temp = *this; --depth_; return temp; }


	float ior() const { return ior_; }
	void ior(float ior) { ior_ = ior; }

private:
	short depth_ = 0;
	float ior_ = 1.f;
};

CRT_END
