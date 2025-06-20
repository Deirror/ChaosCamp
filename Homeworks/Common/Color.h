#pragma once

#include "crt.h"
#include <fstream>

CRT_BEGIN

struct Color {
    float r, g, b;

    void printToFile(std::ofstream&) const;
};

CRT_END