#pragma once

#include "Scene.h"
#include "Color.h"

CRT_BEGIN

Color shade(const HitRecord& hitRecord, const Scene& scene, const std::vector<SceneTriangle>& sceneTriangles);

CRT_END
