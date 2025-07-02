#pragma once

#include <vector>

#include "Vec3.h"
#include "Triangle.h"

namespace gen {
    inline void createPyramid7(
        float baseRadius,
        float height,
        std::vector<crt::Triangle>& faces
    ) {
        const int sides = 7;
        const float angleStep = 2.0f * crt::math::PI / sides;
        const float zOffset = 0.0f;  

        crt::Vec3 apex(0.0f, height, zOffset);

        std::vector<crt::Vec3> baseVerts;
        for (int i = 0; i < sides; ++i) {
            float angle = i * angleStep;
            float x = baseRadius * cos(angle);
            float z = baseRadius * sin(angle) + zOffset;
            baseVerts.emplace_back(x, 0.0f, z);
        }

        for (int i = 0; i < sides; ++i) {
            const crt::Vec3& v1 = baseVerts[i];
            const crt::Vec3& v2 = baseVerts[(i + 1) % sides];
            faces.emplace_back(apex, v1, v2);
        }

        crt::Vec3 baseCenter(0.0f, 0.0f, zOffset);

        for (int i = 0; i < sides; ++i) {
            const crt::Vec3& v1 = baseVerts[i];
            const crt::Vec3& v2 = baseVerts[(i + 1) % sides];
            faces.emplace_back(baseCenter, v2, v1);
        }
    }
}
