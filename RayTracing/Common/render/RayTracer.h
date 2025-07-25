#pragma once

#include <functional>

#include "scene/Scene.h"
#include "scene/SceneRay.h"
#include "image/ImageBuffer.h"
#include "dimension/Interval.h"

#include "HitRecord.h"
#include "Shader.h"

CRT_BEGIN

class RayTracer {
public:
enum class RenderMode {
	Linear,
	RegionBands,
	RegionGrids,
	Buckets
};

enum class AccelerationType {
	Linear,
	AABB,
	KDTree
};

using TraceRayFunc = std::function<HitRecord(const SceneRay&)>;

public:
	RayTracer(Scene* scene, 
		RenderMode renderMode = RenderMode::Linear, 
		AccelerationType accelerationType = AccelerationType::Linear);

	ImageBuffer render() const;

	const Scene* scene() const { return scene_; }
	Scene* scene() { return scene_; }

	void scene(Scene* scene) { CRT_ENSURE(scene != nullptr, "Scene is null pointer"); scene_ = scene; shader_.scene(scene); }

	RenderMode renderMode() const { return renderMode_; }
	void renderMode(RenderMode renderMode) { renderMode_ = renderMode; }

	AccelerationType accelerationType() const { return accelerationType_; }
	void accelerationType(AccelerationType accelerationType) { accelerationType_ = accelerationType; setTraceRayFunc(accelerationType); }

	const TraceRayFunc& traceRayFunc() const { return traceRayFunc_; }

private:
	void renderLinear(ImageBuffer& imageBuffer) const;
	void renderRegionBands(ImageBuffer& imageBuffer) const;
	void renderRegionGrids(ImageBuffer& imageBuffer) const;
	void renderBuckets(ImageBuffer& imageBuffer) const;

	void setTraceRayFunc(AccelerationType accelerationType);

	void traceRays(ImageBuffer& imageBuffer, const Interval& interval) const;

	float updateHitRecord(const SceneRay& ray, const SceneTriangle& sceneTriangle, HitRecord& hitRecord, float closestT) const;

	HitRecord traceRayLinear(const SceneRay& ray) const;
	HitRecord traceRayAABB(const SceneRay& ray) const;
	HitRecord traceRayKDTree(const SceneRay& ray) const;

private:
	Scene* scene_ = nullptr;

	Shader shader_;

	RenderMode renderMode_ = RenderMode::Linear;
	AccelerationType accelerationType_ = AccelerationType::Linear;

	int spp_ = 0;
	TraceRayFunc traceRayFunc_ = nullptr;
};

CRT_END
