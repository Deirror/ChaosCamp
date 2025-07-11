#pragma once

#include "scene/Scene.h"
#include "image/ImageBuffer.h"

#include "HitRecord.h"
#include "Shader.h"

CRT_BEGIN

enum class RenderMode {
	Linear,
	Parallel
};

class RayTracer {
public:
	RayTracer(const Scene* scene, RenderMode renderMode, unsigned short maxDepth = Shader::DEFAULT_MAX_DEPTH) 
		: shader_(scene, this, maxDepth), scene_(scene), renderMode_(renderMode) { CRT_ENSURE(scene != nullptr, "Scene is null pointer"); }

	ImageBuffer render() const;

	const Scene* scene() const { return scene_; }
	void scene(const Scene* scene) { CRT_ENSURE(scene != nullptr, "Scene is null pointer"); scene_ = scene; shader_.scene(scene); }

	RenderMode mode() const { return renderMode_; }
	void mode(RenderMode renderMode) { renderMode_ = renderMode; }

	HitRecord traceRay(const Ray& ray)const;

private:
	void renderLinear(ImageBuffer& imageBuffer) const;
	void renderParallel(ImageBuffer& imageBuffer) const;

	void traceRays(ImageBuffer& imageBuffer, unsigned int startRow, unsigned int endRow) const;

private:
	const Scene* scene_ = nullptr;

	Shader shader_;
	RenderMode renderMode_ = RenderMode::Linear;
};

CRT_END
