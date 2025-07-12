#pragma once

#include "math/Vec3.h"

CRT_BEGIN

class Texture;
struct HitRecord;

Vec3 albedo(const Texture& scene, const HitRecord& hitRecord);

CRT_END
