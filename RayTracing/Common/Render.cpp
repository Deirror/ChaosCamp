#include "Render.h"

#include <thread>

#include "Shade.h"

CRT_BEGIN

static HitRecord findHit(const Ray& ray, const Scene& scene, const std::vector<SceneTriangle>& sceneTriangles, unsigned short depth = 0, unsigned short maxDepth = 3) {
	if (depth++ > maxDepth) {
		return HitRecord(FLT_MAX, Vec3(), Vec3(), Vec3(), 0);
	}

	float closestT = FLT_MAX;
	crt::HitRecord finalHitRecord;

	for (const auto& sceneTriangle : sceneTriangles) {
		crt::HitRecord tempHit;
		const Triangle& triangle = scene.triangle(sceneTriangle.triangleIndex);
		if (triangle.intersect(ray, tempHit) && tempHit.t < closestT) {
			closestT = tempHit.t;
			finalHitRecord = tempHit;
			finalHitRecord.meshIndex = sceneTriangle.meshIndex;
			finalHitRecord.triangleIndex = sceneTriangle.triangleIndex;
		}
	}

	if (closestT == FLT_MAX) {
		return HitRecord(FLT_MAX, Vec3(), Vec3(), Vec3(), 0);
	}

	unsigned int materialIndex = scene.meshes()[finalHitRecord.meshIndex].materialIndex();
	MaterialType materialType = scene.materials()[materialIndex].materialType();

	switch (materialType) {
	case MaterialType::Diffuse:
		break;
	case MaterialType::Reflective:
		Vec3 projection = dot(ray.direction(), finalHitRecord.normal) * finalHitRecord.normal;
		Vec3 reflectDir = ray.direction() - 2.f * projection;

		float bias = math::EPSILON_RAY + math::SLOPE_BIAS * (1 - dot(finalHitRecord.normal, reflectDir.normalized()));
		Ray reflectRay(finalHitRecord.point + finalHitRecord.normal * bias, reflectDir.normalized());

		HitRecord reflectHitRecord = findHit(reflectRay, scene, sceneTriangles, depth);
		reflectHitRecord.materialType = MaterialType::Reflective;

		return reflectHitRecord;
	}

	return finalHitRecord;
}

static void traceRays(const Scene& scene, ImageBuffer& imageBuffer, unsigned int startRow, unsigned int endRow) {
	const std::vector<SceneTriangle>& sceneTriangles = scene.triangles();

	Resolution res = scene.settings().resolution;
	const Camera& camera = scene.camera();

	for (unsigned int y = startRow; y < endRow; ++y) {
		for (unsigned int x = 0; x < res.width(); ++x) {
			float s = static_cast<float>(x) / (res.width() - 1);
			float t = static_cast<float>(y) / (res.height() - 1);
			crt::Ray ray = camera.getRay(s, t);
			
			HitRecord hitRecord = findHit(ray, scene, sceneTriangles);

			if (hitRecord.t != FLT_MAX) {
				imageBuffer.set(x, y, shade(hitRecord , scene));
			}
			else {
				Color albedo(255, 255, 255);
				switch (hitRecord.materialType) {
				case MaterialType::Reflective:
					unsigned int materialIndex = scene.meshes()[hitRecord.meshIndex].materialIndex();
					albedo = scene.materials()[materialIndex].albedo();
					break;
				}

				Color backgroundColor = scene.settings().backgroundColor;
				Color finalColor(
					backgroundColor.r * albedo.r / 255.f, 
					backgroundColor.g * albedo.g / 255.f,
					backgroundColor.b * albedo.b / 255.f
				);
				imageBuffer.set(x, y, finalColor);
			}
		}
	}
}

ImageBuffer Render::render(const Scene& scene) const {
	ImageBuffer buffer(scene.settings().resolution);

	switch (renderMode_) {
	case RenderMode::Linear:
		renderLinear(scene, buffer);
		break;
	case RenderMode::Parallel:
		renderParallel(scene, buffer);
		break;
	}

	return buffer;
}

void Render::renderLinear(const Scene& scene, ImageBuffer& imageBuffer) const {
	traceRays(scene, imageBuffer, 0, scene.settings().resolution.height());
}

void Render::renderParallel(const Scene& scene, ImageBuffer& imageBuffer) const {
	const Resolution res = scene.settings().resolution;

	unsigned int threadCount = std::thread::hardware_concurrency();
	unsigned int bandHeight = res.height() / threadCount;

	std::vector<std::thread> threads;
	for (unsigned int i = 0; i < threadCount; ++i) {
		unsigned int startRow = i * bandHeight;
		unsigned int endRow = (i == threadCount - 1) ? res.height() : (i + 1) * bandHeight;
		threads.emplace_back(traceRays,
			std::cref(scene),
			std::ref(imageBuffer),
			startRow,
			endRow);
	}

	for (auto& t : threads) {
		t.join();
	}
}

CRT_END
