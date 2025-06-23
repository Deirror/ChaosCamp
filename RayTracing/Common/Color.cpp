#include "Color.h"

#include <random>
#include <algorithm>

CRT_BEGIN

Color getRandomColor(int maxColorComponent) {
    unsigned char r = rand() % (maxColorComponent + 1);
    unsigned char g = rand() % (maxColorComponent + 1);
    unsigned char b = rand() % (maxColorComponent + 1);

    return crt::Color{ r, g, b };
}

CRT_END
