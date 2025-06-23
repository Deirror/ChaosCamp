#pragma once

#include "Vec3.h"

CRT_BEGIN

class Ray {
public:
    Ray() = default;
    Ray(const Vec3& origin, const Vec3& direction)
        : m_Origin(origin), m_Direction(direction) {}

    const Vec3& getOrigin() const { return m_Origin; }
    const Vec3& getDirection() const { return m_Direction; }

    void setOrigin(const Vec3& origin) { m_Origin = origin;	}
    void setDirection(const Vec3& direction) { m_Direction = direction; }

    Vec3 at(float scalar) const;

private:
    Vec3 m_Origin;
    Vec3 m_Direction;
};

CRT_END