#pragma once

#include "core/error.h"
#include "image/Color.h"
#include "HitRecord.h"

CRT_BEGIN

class RayTracer;
class PathRay;
class Scene;

struct RayEffects {
	bool reflection = false;
	bool refraction = false;
};

class Shader {
public:
	Shader(const Scene* scene, const RayTracer* rayTracer);

	const Scene* scene() const { return scene_; }
	void scene(const Scene* scene);

	Vec3 shade(PathRay& pathRay, const HitRecord& hitRecord) const;

public:
	static constexpr int kDefaultMaxDepth = 4;

private:
	Vec3 shadeDiffuse(PathRay& pathRay, const HitRecord& hitRecord) const;
	Vec3 shadeDirectIllumination(PathRay& pathRay, const HitRecord& hitRecord) const;
	Vec3 shadeGlobalIllumination(PathRay& pathRay, const HitRecord& hitRecord) const;

	Vec3 shadeReflective(PathRay& pathRay, const HitRecord& hitRecord) const;
	Vec3 shadeRefractive(PathRay& pathRay, const HitRecord& hitRecord) const;
	Vec3 shadeConstant(PathRay& pathRay, const HitRecord& hitRecord) const;

	Vec3 sample(const HitRecord& hitRecord) const;
	Vec3 sampleRandomHemisphere() const;

	void updateRayParams();

private:
	const Scene* scene_ = nullptr;
	const RayTracer* rayTracer_ = nullptr;

	int giRays_ = 0;
	int maxDepth_ = kDefaultMaxDepth;

	RayEffects rayEffects;
};

CRT_END
