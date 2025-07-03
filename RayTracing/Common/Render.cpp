#include "Render.h"

#include "PPMFile.h"
#include "Math/Hit.h"

CRT_BEGIN

void Render::render(const Scene& scene) const {
	std::vector<Triangle> triangles = scene.getTriangles();
	std::vector<Color> colors;
	for (int i = 0; i < triangles.size(); ++i) {
		colors.push_back(getRandomColor());
	}
	Resolution resolution = scene.settings().resolution;
	const Camera& camera = scene.camera();

	PPMFile ppmFile("task1_image.ppm", resolution);

	for (int y = 0; y < resolution.height(); ++y) {
		for (int x = 0; x < resolution.width(); ++x) {
			float s = static_cast<float>(x) / (resolution.width() - 1);
			float t = static_cast<float>(y) / (resolution.height() - 1);
			crt::Ray ray = camera.getRay(s, t);

			float closestT = FLT_MAX;

			crt::HitRecord hitRecord;
			for (const auto& triangle : triangles) {
				if (crt::intersectTriangle(ray, triangle, hitRecord)) {
					closestT = std::min(closestT, hitRecord.t);
				}
			}

			if (closestT != FLT_MAX) {
				int idx = static_cast<int>(hitRecord.triangle - &triangles[0]);
				ppmFile.printColor(colors.at(idx));
			}
			else {
				ppmFile.printColor(scene.settings().backgroundColor);
			}
		}
		ppmFile.addNewLine();
	}
}

CRT_END
