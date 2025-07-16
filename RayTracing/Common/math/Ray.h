#pragma once

#include "Vec3.h"

CRT_BEGIN

enum class RayType {
    Camera,
    Shadow,
    Reflective,
    Refractive
};

class Ray {
public:
    Ray() = default;
    Ray(const Vec3& origin, const Vec3& direction, RayType rayType = RayType::Camera)
        : origin_(origin), direction_(direction), rayType_(rayType) {}

    const Vec3& origin() const { return origin_; }
    void origin(const Vec3& origin) { origin_ = origin;	}
    
    const Vec3& direction() const { return direction_; }
    void direction(const Vec3& direction) { direction_ = direction; }

    RayType rayType() const { return rayType_; }
    void rayType(RayType rayType) { rayType_ = rayType; }

    Vec3 at(float t) const { return origin_ + t * direction_; }

protected:
    Vec3 origin_;
    Vec3 direction_;

    RayType rayType_ = RayType::Camera;
};

CRT_END