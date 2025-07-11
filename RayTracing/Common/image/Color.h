#pragma once

#include <fstream>

#include "math/Vec3.h"

CRT_BEGIN

struct Color {
    unsigned char r = 0, g = 0, b = 0;
};

Color fromVec3ToColor(const Vec3& vec);
Vec3 fromColorToVec3(Color color);

Color getRandomColor(unsigned char maxColorComponent = 255);

CRT_END