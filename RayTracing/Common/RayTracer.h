#pragma once

#include "crt.h"
#include "Scene.h"

#include "ImageBuffer.h"

CRT_BEGIN

enum class RenderMode {
	Linear,
	Parallel
};

class RayTracer {
public:
	RayTracer() = default;
	RayTracer(RenderMode renderMode) : renderMode_(renderMode) {}

	ImageBuffer render(const Scene& scene) const;

	RenderMode mode() const { return renderMode_; }
	void mode(RenderMode renderMode) { renderMode_ = renderMode; }

private:
	void renderLinear(const Scene& scene, ImageBuffer& imageBuffer) const;
	void renderParallel(const Scene& scene, ImageBuffer& imageBuffer) const;

private:
	RenderMode renderMode_ = RenderMode::Linear;
};

CRT_END
