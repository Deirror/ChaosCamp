#pragma once

#include "Vec3.h"

CRT_BEGIN

class Ray {
public:
    Ray() = default;
    Ray(const Vec3& origin, const Vec3& direction)
        : origin_(origin), direction_(direction) {}

    const Vec3& origin() const { return origin_; }
    void origin(const Vec3& origin) { origin_ = origin;	}
    
    const Vec3& direction() const { return direction_; }
    void direction(const Vec3& direction) { direction_ = direction; }

    Vec3 at(float t) const { return origin_ + t * direction_; }

private:
    Vec3 origin_;
    Vec3 direction_;
};

CRT_END