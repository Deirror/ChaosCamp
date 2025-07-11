#include "Viewport.h"

CRT_BEGIN

void Viewport::update(const Vec3& cameraPosition, const Mat3& rotation, float focalLength) {
    float width = aspectHeight_.height * aspectHeight_.ratio;

    Vec3 u = rotation.row0();
    Vec3 v = rotation.row1();
    Vec3 w = rotation.row2();

    horizontal_ = width * u;
    vertical_ = -aspectHeight_.height * v;

    lowerLeftCorner_ = cameraPosition - (focalLength * w)
        - 0.5f * horizontal_
        - 0.5f * vertical_;
}

CRT_END

