#pragma once

#include "math/Vec3.h"
#include "scene/Material.h"

CRT_BEGIN

struct HitRecord {
	float t = FLT_MAX;
	Vec3 point;
	Vec3 faceNormal;
	Vec3 hitNormal;
	Vec3 barycentricCoords;
	Vec3 puv;

	unsigned int triangleIndex = 0;
	unsigned int meshIndex = 0;
	unsigned int materialIndex = 0;
	unsigned int textureIndex = 0;
};

CRT_END
