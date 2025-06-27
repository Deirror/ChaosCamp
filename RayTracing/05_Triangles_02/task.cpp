#include <array>
#include <vector>

#include "Triangle.h"
#include "PPMFile.h"
#include "Hit.h"

void task() {
	float aspectRatio = 6.0f / 4.0f;
	int width = 1440;

	crt::Resolution resolution(width, width / aspectRatio);
	crt::PPMFile file("star7.ppm", resolution);

	crt::Color backgroundColor{ 0, 166, 0 };
	
	crt::Vec3 center(0.0f, 0.0f, -4.0f);
	std::vector<crt::Vec3> vertices;
	vertices.push_back(center);

	const int PEAKS = 7;
	const int VERTICES = PEAKS * 2;
	const float OUTER_RADIUS = 1.0f;
	const float INNER_RADIUS = 0.5f;

	for (int i = 0; i < VERTICES; ++i) {
		float angle = (2.0f * 3.14f / VERTICES) * i;
		float radius = (i % 2 == 0) ? OUTER_RADIUS : INNER_RADIUS;
		float x = radius * std::cos(angle);
		float y = radius * std::sin(angle);
		float z = -3.0f;
		vertices.emplace_back(x, y, z);
	}

	std::vector<crt::Triangle> triangles;
	for (int i = 1; i <= VERTICES; ++i) {
		int next = (i % VERTICES) + 1;
		triangles.emplace_back(center, vertices[i], vertices[next]);
	}

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

	float focalLen = 1.0f;
	float viewportHeight = 2.0f;
	float viewportWidth = viewportHeight /
		(float(resolution.width()) / resolution.height());

	crt::Vec3 cameraCenter = crt::Vec3(0.0f, 0.0f, 0.0f);

	crt::Vec3 viewportU = crt::Vec3(viewportWidth, 0.0f, 0.0f);
	crt::Vec3 viewportV = crt::Vec3(0.0f, -viewportHeight, 0.0f);

	crt::Vec3 pixelDeltaU = viewportU / resolution.width();
	crt::Vec3 pixelDeltaV = viewportV / resolution.height();

	crt::Vec3 viewportUpperLeft = cameraCenter - crt::Vec3(0.0f, 0.0f, focalLen) -
		(viewportU / 2.0f) - (viewportV / 2.0f);

	crt::Vec3 pixelLoc = viewportUpperLeft + 0.5f * (pixelDeltaU + pixelDeltaV);

	for (int j = 0; j < resolution.height(); j++) {
		for (int i = 0; i < resolution.width(); i++) {
			crt::Vec3 pixelCenter = pixelLoc + (i * pixelDeltaU) + (j * pixelDeltaV);
			crt::Vec3 rayDirection = pixelCenter - cameraCenter;
			crt::Ray ray(cameraCenter, rayDirection);

			crt::HitRecord hitRecord;
			crt::Color finalColor = backgroundColor;

			float t_closest = FLT_MAX; 

			for (auto& triangle : triangles) {
				if (crt::intersectTriangle(ray, triangle, hitRecord)) {
					if (hitRecord.t < t_closest) {
						finalColor = colors[&triangle - &triangles[0]];
						t_closest = hitRecord.t;
					}
				}
			}
			file.printColor(finalColor);
		}
	}
}
