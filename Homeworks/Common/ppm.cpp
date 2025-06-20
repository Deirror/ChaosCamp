#include "ppm.h"

CRT_BEGIN

void PPMHeader::setPPMFileHeader(std::ofstream& ppmFile) const {
    ppmFile << "P3\n";
    ppmFile << width << " " << height << '\n'; 
    ppmFile << static_cast<int>(maxColorComponent) << '\n';
}

CRT_END