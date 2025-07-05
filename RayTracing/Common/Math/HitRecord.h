#pragma once

#include "Vec3.h"
#include "Material.h"

CRT_BEGIN

class Triangle;

struct HitRecord {
	float t = FLT_MAX;
	Vec3 point;
	Vec3 normal;
	Vec3 barycentricCoords;
	const Triangle* triangle;
	unsigned int meshIndex = 0;
	MaterialType materialType = MaterialType::DIFFUSE;
};

CRT_END
