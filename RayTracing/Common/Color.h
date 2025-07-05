#pragma once

#include <fstream>

#include "Math/Vec3.h"

CRT_BEGIN

struct Color {
    unsigned char r, g, b;
};

Color fromVec3ToColor(const Vec3& vec);

Color getRandomColor(unsigned char maxColorComponent = 255);

CRT_END