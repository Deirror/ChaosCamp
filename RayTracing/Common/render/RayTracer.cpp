#include "RayTracer.h"

#include <thread>

#include "Intersection.h"
#include "PathRay.h"

CRT_BEGIN

ImageBuffer RayTracer::render() const {
	ImageBuffer buffer(scene_->settings().resolution);

	switch (renderMode_) {
	case RenderMode::Linear:
		renderLinear(buffer);
		break;
	case RenderMode::Parallel:
		renderParallel(buffer);
		break;
	}

	return buffer;
}

void RayTracer::traceRays(ImageBuffer& imageBuffer, unsigned int startRow, unsigned int endRow) const {
	Resolution res = scene_->settings().resolution;
	const Camera& camera = scene_->camera();

	for (unsigned int y = startRow; y < endRow; ++y) {
		for (unsigned int x = 0; x < res.width(); ++x) {
			float s = static_cast<float>(x) / (res.width() - 1);
			float t = static_cast<float>(y) / (res.height() - 1);
			crt::Ray ray = camera.getRay(s, t);
			
			HitRecord hitRecord = traceRay(ray);

			PathRay pathRay(ray);
			Color color = shader_.shade(pathRay, hitRecord);
			imageBuffer.set(x, y, color);
		}
	}
}

HitRecord RayTracer::traceRay(const Ray& ray) const {
	float closestT = FLT_MAX;
	crt::HitRecord finalHitRecord;

	for (const auto& sceneTriangle : scene_->sceneTriangles()) {
		crt::HitRecord tempHit;
		const Triangle& triangle = scene_->triangle(sceneTriangle.triangleIndex);

		bool cullBackFace = (ray.rayType() == RayType::Camera);

		if (intersect(triangle, ray, tempHit, cullBackFace) && tempHit.t < closestT) {
			closestT = tempHit.t;
			finalHitRecord = tempHit;
			finalHitRecord.meshIndex = sceneTriangle.meshIndex;
			finalHitRecord.triangleIndex = sceneTriangle.triangleIndex;
			finalHitRecord.materialIndex = scene_->mesh(sceneTriangle.meshIndex).materialIndex();
		}
	}

	return finalHitRecord;
}

void RayTracer::renderLinear(ImageBuffer& imageBuffer) const {
	CRT_ENSURE(renderMode_ == RenderMode::Linear, "Render mode is not linear");
	traceRays(imageBuffer, 0, scene_->settings().resolution.height());
}

void RayTracer::renderParallel(ImageBuffer& imageBuffer) const {
	CRT_ENSURE(renderMode_ == RenderMode::Parallel, "Render mode is not parallel");

	const Resolution res = scene_->settings().resolution;

	unsigned int threadCount = std::thread::hardware_concurrency();
	unsigned int bandHeight = res.height() / threadCount;

	std::vector<std::thread> threads;
	for (unsigned int i = 0; i < threadCount; ++i) {
		unsigned int startRow = i * bandHeight;
		unsigned int endRow = (i == threadCount - 1) ? res.height() : (i + 1) * bandHeight;
		threads.emplace_back(&RayTracer::traceRays,
			this,
			std::ref(imageBuffer),
			startRow,
			endRow);
	}

	for (auto& t : threads) {
		t.join();
	}
}

CRT_END
