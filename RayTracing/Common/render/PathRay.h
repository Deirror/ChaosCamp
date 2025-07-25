#pragma once

#include "scene/SceneRay.h"

CRT_BEGIN

class PathRay : public SceneRay {
public:
	PathRay(const Vec3& origin, const Vec3& direction, float ior = 1.f) 
		: SceneRay(origin, direction), ior_(ior) {}

	PathRay(const SceneRay& sceneRay, float ior = 1.f) 
		: SceneRay(sceneRay), ior_(ior) {}

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
