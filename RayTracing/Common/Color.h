#pragma once

#include "crt.h"
#include <fstream>

CRT_BEGIN

struct Color {
    unsigned char r, g, b;
};

Color getRandomColor(unsigned char maxColorComponent = 255);

CRT_END