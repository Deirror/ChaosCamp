#include "Ray.h"

CRT_BEGIN

Vec3 Ray::at(float t) const {
    return origin_ + t * direction_;
}

CRT_END