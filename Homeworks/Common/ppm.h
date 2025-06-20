#pragma once

#include <fstream>
#include "crt.h"

CRT_BEGIN

struct PPMHeader {
    int width;
    int height;
    unsigned char maxColorComponent; // max can be 255 so it fits well in unsigned char

    void setPPMFileHeader(std::ofstream&) const;
};

CRT_END