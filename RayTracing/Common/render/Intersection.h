#pragma once

#include "core/crt.h"

CRT_BEGIN

class Triangle;
class Ray;

struct HitRecord;

bool intersect(const Triangle& triangle, const Ray& ray, HitRecord& hitRecord, bool cullBackFace = true);

CRT_END