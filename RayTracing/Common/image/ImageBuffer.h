#pragma once

#include <vector>
#include <sstream>

#include "dimensions/Resolution.h"

#include "Color.h"
#include "PPMFile.h"

CRT_BEGIN

class ImageBuffer {
public:
    ImageBuffer() = default;
    ImageBuffer(unsigned short width, unsigned short height)
        : resolution_(width, height), pixels_(width * height) {}

    ImageBuffer(Resolution resolution)
        : resolution_(resolution), pixels_(resolution.width() * resolution.height()) {}

    void set(unsigned short x, unsigned short y, Color color);

    void save(const std::string& filename, PPMMode ppmMode = PPMMode::P3) const;
    
    void dumpASCII(PPMFile& ppmFile) const;
    void dumpBinary(PPMFile& ppmFile) const;

    unsigned short width() const { return resolution_.width(); }
    unsigned short height() const { return resolution_.height(); }

    Color at(unsigned short x, unsigned short y) const;

    const std::vector<Color>& pixels() const { return pixels_; }

private:
    Resolution resolution_;
    std::vector<Color> pixels_;
};

CRT_END
