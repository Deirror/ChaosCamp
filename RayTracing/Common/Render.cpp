#include "Render.h"

#include <thread>

#include "Math/Hit.h"

CRT_BEGIN

static void rayTrace(const Scene& scene, ImageBuffer& imageBuffer, const std::vector<Triangle>& triangles, const std::vector<Color>& colors, unsigned int startRow, unsigned int endRow) {
	Resolution res = scene.settings().resolution;
	const Camera& camera = scene.camera();

	for (int y = startRow; y < endRow; ++y) {
		for (int x = 0; x < res.width(); ++x) {
			float s = static_cast<float>(x) / (res.width() - 1);
			float t = static_cast<float>(y) / (res.height() - 1);
			crt::Ray ray = camera.getRay(s, t);

			float closestT = FLT_MAX;
			crt::HitRecord hitRecord;

			for (const auto& triangle : triangles) {
				if (crt::intersectTriangle(ray, triangle, hitRecord)) {
					closestT = std::min(closestT, hitRecord.t);
				}
			}

			if (closestT != FLT_MAX) {
				unsigned int idx = static_cast<unsigned int>(hitRecord.triangle - &triangles[0]);
				imageBuffer.set(x, y, colors.at(idx));
			}
			else {
				imageBuffer.set(x, y, scene.settings().backgroundColor);
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
	std::vector<Triangle> triangles = scene.getTriangles();
	std::vector<Color> colors;

	for (int i = 0; i < triangles.size(); ++i) {
		colors.push_back(getRandomColor());
	}

	rayTrace(scene, imageBuffer, triangles, colors, 0, scene.settings().resolution.height());
}

void Render::renderParallel(const Scene& scene, ImageBuffer& imageBuffer) const {
	std::vector<Triangle> triangles = scene.getTriangles();
	std::vector<Color> colors;

	for (int i = 0; i < triangles.size(); ++i) {
		colors.push_back(getRandomColor());
	}

	const Resolution res = scene.settings().resolution;

	unsigned int threadCount = std::thread::hardware_concurrency();
	unsigned int bandHeight = res.height() / threadCount;

	std::vector<std::thread> threads;
	for (unsigned int i = 0; i < threadCount; ++i) {
		unsigned int startRow = i * bandHeight;
		unsigned int endRow = (i == threadCount - 1) ? res.height() : (i + 1) * bandHeight;
		threads.emplace_back(rayTrace,
			std::cref(scene),
			std::ref(imageBuffer),
			std::cref(triangles),
			std::cref(colors),
			startRow,
			endRow);
	}

	for (auto& t : threads) {
		t.join();
	}
}

CRT_END
