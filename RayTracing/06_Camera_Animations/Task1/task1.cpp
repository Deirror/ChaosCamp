#include <vector>

#include "PPMFile.h"
#include "Camera.h"
#include "Math/Triangle.h"
#include "Math/Hit.h"

#include "Math/utils.h"
#include "../common.h"

void task1() {
	crt::Resolution resolution(800, 600);

	crt::Camera camera(
		crt::Vec3(0.0f, 0.0f, 0.0f),
		crt::Vec3(0.0f, 0.0f, -2.0f),
		crt::Vec3(0.0f, 1.0f, 0.0f),
		resolution
	);

	crt::PPMFile ppmFile("task1_image.ppm", resolution);

	std::vector<crt::Triangle> triangles;
	gen::createPyramid7(1.0f, 2, triangles);

	crt::Color backgroundColor{ 0, 125, 0 };

	camera.pan(crt::math::to_radians(30));

	std::array<crt::Color, 14> colors = {
		crt::Color{ 230, 25, 75 },   // red
		crt::Color{ 60, 180, 75 },   // green
		crt::Color{ 255, 225, 25 },  // yellow
		crt::Color{ 0, 130, 200 },   // blue
		crt::Color{ 245, 130, 48 },  // orange
		crt::Color{ 145, 30, 180 },  // purple
		crt::Color{ 70, 240, 240 },  // cyan
		crt::Color{ 240, 50, 230 },  // pink
		crt::Color{ 210, 245, 60 },  // lime
		crt::Color{ 250, 190, 190 }, // light pink
		crt::Color{ 0, 128, 128 },   // teal
		crt::Color{ 230, 190, 255 }, // lavender
		crt::Color{ 170, 110, 40 },  // brown
		crt::Color{ 255, 250, 200 }  // light yellow
	};

	for (int y = 0; y < camera.height(); ++y) {
		for (int x = 0; x < camera.width(); ++x) {
			float s = static_cast<float>(x) / (camera.width() - 1);
			float t = static_cast<float>(y) / (camera.height() - 1);
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
				ppmFile.printColor(backgroundColor);
			}
		}
		ppmFile.addNewLine();
	}
}
