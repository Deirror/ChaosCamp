#pragma once

#include "Vec3.h"

CRT_BEGIN

class Triangle;

struct HitRecord {
	float t;
	Vec3 point;
	Vec3 normal;
	const Triangle* triangle;
};

CRT_END
