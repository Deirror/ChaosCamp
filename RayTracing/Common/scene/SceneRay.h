#pragma once

#include "math/Ray.h"

CRT_BEGIN

enum class RayType {
	Camera,
	Shadow,
	Reflective,
	Refractive
};

class SceneRay : public Ray {
public:
	SceneRay(const Vec3& origin, const Vec3& direction, RayType type = RayType::Camera)
		: Ray(origin, direction), type_(type) {
	}

	SceneRay(const Ray& ray, RayType type = RayType::Camera)
		: Ray(ray), type_(type) {
	}

	RayType type() const { return type_; }
	void type(RayType type) { type_ = type; }

private:
	RayType type_ = RayType::Camera;
};

CRT_END