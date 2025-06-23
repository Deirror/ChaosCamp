#include "Ray.h"

CRT_BEGIN

Vec3 Ray::at(float scalar) const {
    return m_Origin + scalar * m_Direction;
}

CRT_END