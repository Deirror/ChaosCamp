#pragma once

#include "Vec3.h"
#include "Material.h"

CRT_BEGIN

class Mesh;

struct HitRecord {
	float t = FLT_MAX;
	Vec3 point;
	Vec3 faceNormal;
	Vec3 hitNormal;
	Vec3 barycentricCoords;
	unsigned int triangleIndex = 0;
	unsigned int meshIndex = 0;
	MaterialType materialType = MaterialType::Diffuse;
};

CRT_END
