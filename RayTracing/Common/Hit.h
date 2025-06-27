#pragma once

#include "Triangle.h"

CRT_BEGIN

struct HitRecord {
	float t;
	Vec3 point;
	Vec3 normal;
	const Triangle* triangle;
};

bool intersectTriangle(const Ray& ray, const Triangle& triangle, HitRecord& hitRecord);

CRT_END
