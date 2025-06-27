#include <fstream>
#include <cmath>
#include <algorithm>
#include <iostream>

#include "Vec3.h"
#include "PPMFile.h"
#include "Color.h"
#include "Ray.h"

static crt::Vec3 horizontalColor(const crt::Ray& ray) {
    crt::Vec3 unitDirection = ray.direction().normalized();
    float a = 0.5f * (unitDirection.y() + 1.0f);
    return (1.0f - a) * crt::Vec3(1.0f, 1.0f, 1.0f) + a * crt::Vec3(0.5f, 0.7f, 1.0f);
}

static crt::Color unitColor(const crt::Ray& ray) {
    crt::Vec3 unitDirection = ray.direction().normalized();

    unsigned char r = static_cast<unsigned char>(255.999f * std::abs(unitDirection.x()));
    unsigned char g = static_cast<unsigned char>(255.999f * std::abs(unitDirection.y()));
    unsigned char b = static_cast<unsigned char>(255.999f * std::abs(unitDirection.z()));
    
    return crt::Color{r, g, b};
}

void task1() {
    int width = 1760;
    float aspectRatio = 16.0f / 9.0f;

    int height = std::max(int(width / aspectRatio), 1);

    crt::Resolution resolution(width, height);

    float focalLen = 1.0f;
    float viewportHeight = 2.0f;
    float viewportWidth = viewportHeight /
        (float(width) / height);

    crt::Vec3 cameraCenter = crt::Vec3(0.0f, 0.0f, 0.0f);

    crt::Vec3 viewportU = crt::Vec3(viewportWidth, 0.0f, 0.0f);
    crt::Vec3 viewportV = crt::Vec3(0.0f, -viewportHeight, 0.0f);

    crt::Vec3 pixelDeltaU = viewportU / width;
    crt::Vec3 pixelDeltaV = viewportV / height;

    crt::Vec3 viewportUpperLeft = cameraCenter - crt::Vec3(0.0f, 0.0f, focalLen) -
        (viewportU / 2.0f) - (viewportV / 2.0f);

    crt::Vec3 pixelLoc = viewportUpperLeft + 0.5f * (pixelDeltaU + pixelDeltaV);

    crt::PPMFile file("task1_image.ppm", resolution);

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            crt::Vec3 pixelCenter = pixelLoc + (i * pixelDeltaU) + (j * pixelDeltaV);
            crt::Vec3 rayDirection = pixelCenter - cameraCenter;
            crt::Ray ray(cameraCenter, rayDirection);

            crt::Vec3 pixelColor = horizontalColor(ray) * 255.999f;
            crt::Color color = { static_cast<unsigned char>(pixelColor.x()), static_cast<unsigned char>(pixelColor.y()), static_cast<unsigned char>(pixelColor.z())};
            file.printColor(color);
        }
        file.addNewLine();
    }
}