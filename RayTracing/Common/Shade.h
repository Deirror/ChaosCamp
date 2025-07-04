#pragma once

#include "Scene.h"
#include "Color.h"

CRT_BEGIN

Color shade(const HitRecord& hitRecord, const Scene& scene);

CRT_END
