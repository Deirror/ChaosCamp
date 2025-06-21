#pragma once

#include <fstream>
#include "crt.h"

CRT_BEGIN

struct PPMHeader {
    int width;
    int height;
    unsigned char maxColorComponent;

    void setPPMFileHeader(std::ofstream&) const;
};

CRT_END