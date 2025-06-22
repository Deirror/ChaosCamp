#include "Color.h"

CRT_BEGIN

void Color::printToFile(std::ofstream& ppmFile) const {
    ppmFile << r << ' ' << g << ' ' << b << '\t';
}

CRT_END