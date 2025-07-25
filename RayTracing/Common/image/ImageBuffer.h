#pragma once

#include <vector>
#include <sstream>

#include "dimension/Resolution.h"

#include "Color.h"
#include "PPMWriter.h"

CRT_BEGIN

class ImageBuffer {
public:
    ImageBuffer() = default;
    ImageBuffer(const Resolution& resolution)
        : resolution_(resolution), pixels_(resolution.width() * resolution.height()) {}

    void set(int x, int y, const Color& color);

    void save(const std::string& filepath, PPMWriter::Format format = PPMWriter::Format::P3) const;
    
    void dumpASCII(PPMWriter& file) const;
    void dumpBinary(PPMWriter& file) const;

    int width() const { return resolution_.width(); }
    int height() const { return resolution_.height(); }

    const Color& at(int x, int y) const;

    const std::vector<Color>& pixels() const { return pixels_; }

private:
    Resolution resolution_;
    std::vector<Color> pixels_;
};

CRT_END
