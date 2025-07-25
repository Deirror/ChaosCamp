#pragma once

#include "core/crt.h"

CRT_BEGIN

class Triangle;
class AABB;
class Ray;

struct HitRecord;

bool intersect(const Triangle& triangle, const Ray& ray, HitRecord& hitRecord, bool cullBackFace = true);
bool intersect(const AABB& aabb, const Ray& ray);
bool intersect(const AABB& lhs, const AABB& rhs);

CRT_END