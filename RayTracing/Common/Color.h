#pragma once

#include "crt.h"
#include <fstream>

CRT_BEGIN

struct Color {
    unsigned char r, g, b;
};

Color getRandomColor(int maxColorComponent);

CRT_END