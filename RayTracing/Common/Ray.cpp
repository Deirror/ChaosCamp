#include "Ray.h"

CRT_BEGIN

Vec3 Ray::at(float scalar) const {
    return origin_ + scalar * direction_;
}

CRT_END