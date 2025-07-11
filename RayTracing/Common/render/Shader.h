#pragma once

#include "core/error.h"
#include "image/Color.h"
#include "HitRecord.h"

CRT_BEGIN

class RayTracer;
class PathRay;
class Scene;

class Shader {
public:
	Shader(const Scene* scene, const RayTracer* rayTracer, unsigned short maxDepth = DEFAULT_MAX_DEPTH);

	const Scene* scene() const { return scene_; }
	void scene(const Scene* scene) { CRT_ENSURE(scene != nullptr, "Scene is null pointer"); scene_ = scene; }

	Color shade(PathRay& pathRay, const HitRecord& hitRecord) const;

	unsigned short maxDepth() const { return maxDepth_; }
	void maxDepth(unsigned short maxDepth) { maxDepth_ = maxDepth; }

public:
	static constexpr unsigned short DEFAULT_MAX_DEPTH = 5;

private:
	Color shadeDiffuse(PathRay& pathRay, const HitRecord& hitRecord) const;
	Color shadeReflective(PathRay& pathRay, const HitRecord& hitRecord) const;
	Color shadeRefractive(PathRay& pathRay, const HitRecord& hitRecord) const;
	Color shadeConstant(PathRay& pathRay, const HitRecord& hitRecord) const;

private:
	const Scene* scene_ = nullptr;
	const RayTracer* rayTracer_ = nullptr;

	unsigned short maxDepth_ = DEFAULT_MAX_DEPTH;
};

CRT_END
