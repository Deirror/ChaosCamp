#include <fstream>
#include <cmath>

#include "Common/Vector.hpp"
#include "Common/ppm.h"
#include "Common/Color.h"
#include "Common/Ray.h"

int main() {
    int width = 1760;
    float aspectRatio = 16.0f / 9.0f;

    int height = std::min(int(width / aspectRatio), 1);

    unsigned char maxColorComponent = 255;

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

    // Pixel's center
    crt::Vec3 pixelLoc = viewportUpperLeft + 0.5f * (pixelDeltaU + pixelDeltaV);

    crt::PPMHeader header{width, height, maxColorComponent};

    std::ofstream ppmFile("unit_rays.ppm", std::ios::out|std::ios::binary);
    header.setPPMFileHeader(ppmFile);

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            crt::Vec3 pixelCenter = pixelLoc + (i * pixelDeltaU) + (j * pixelDeltaV);
            crt::Vec3 ray_direction = pixelCenter - cameraCenter;
            crt::Ray r(cameraCenter, ray_direction);

            crt::Vec3 unitRayDir = r.m_Direction.normalized(); 

            unitRayDir *= 2.0f;
            unitRayDir -= 1.0f;
            unitRayDir *= 255.0f;

            crt::Color pixelColor = {unitRayDir[0], unitRayDir[1], unitRayDir[2]};
            pixelColor.printToFile(ppmFile);
        }
        ppmFile << '\n';
    }

    return 0;
}