#pragma once

#include <functional>

#include "scene/Scene.h"
#include "image/ImageBuffer.h"
#include "dimensions/Interval.h"

#include "HitRecord.h"
#include "Shader.h"

CRT_BEGIN

enum class RenderMode {
	Linear,
	RegionBands,
	RegionGrids,
	Buckets
};

enum class AccelerationType {
	Linear,
	AABB
};

using TraceRayFunc = std::function<HitRecord(const Ray&)>;

class RayTracer {
public:
	RayTracer(const Scene* scene, RenderMode renderMode = RenderMode::Linear, AccelerationType accelerationType = AccelerationType::Linear, unsigned short maxDepth = Shader::DEFAULT_MAX_DEPTH);

	ImageBuffer render() const;

	const Scene* scene() const { return scene_; }
	void scene(const Scene* scene) { CRT_ENSURE(scene != nullptr, "Scene is null pointer"); scene_ = scene; shader_.scene(scene); }

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

	float updateHitRecord(const Ray& ray, const SceneTriangle& sceneTriangle, HitRecord& hitRecord, float closestT) const;

	HitRecord traceRayLinear(const Ray& ray) const;
	HitRecord traceRayAABB(const Ray& ray) const;

private:
	const Scene* scene_ = nullptr;

	Shader shader_;

	RenderMode renderMode_ = RenderMode::Linear;
	AccelerationType accelerationType_ = AccelerationType::Linear;

	TraceRayFunc traceRayFunc_ = nullptr;
};

CRT_END
