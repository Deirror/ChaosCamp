#include "Viewport.h"

CRT_BEGIN

void Viewport::update(const Vec3& cameraPosition, const Mat3& rotation, float focalLength, float fov) {
    float viewportHeight = 2.f * focalLength * std::tan(fov / 2.f);
    float viewportWidth = viewportHeight * aspectHeight_.ratio; 

    Vec3 u = rotation.row0(); 
    Vec3 v = rotation.row1();
    Vec3 w = rotation.row2(); 

    horizontal_ = viewportWidth * u;
    vertical_ = -viewportHeight * v;

    lowerLeftCorner_ = cameraPosition
        - (focalLength * w)
        - 0.5f * horizontal_
        - 0.5f * vertical_;
}

CRT_END

