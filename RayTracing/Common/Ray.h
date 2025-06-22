#pragma once

#include "Vector.hpp"

CRT_BEGIN

class Ray {
public:
    Ray() = default;
    Ray(const Vec3& origin, const Vec3& direction);

    const Vec3& origin() const;
    const Vec3& direction() const;

    Vec3 at(double t) const;

    Vec3 m_Origin;
    Vec3 m_Direction;
};

CRT_END