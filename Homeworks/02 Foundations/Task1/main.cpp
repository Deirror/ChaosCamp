#include <fstream>
#include <random>
#include <array>

#include <iostream>

#include "Common/ppm.h"
#include "Common/Color.h"

static void drawImageWithRectangles(std::ofstream&, const crt::PPMHeader&);
static crt::Color getRandomColor(crt::Color, int); 
static void printColorToPPMFile(std::ofstream&, crt::Color);

static crt::Color getRandomColor(crt::Color, int&);

int main() {
    const int imageWidth = 1720,
            imageHeight = 1240,
            maxColorCOmponent = 255;

    crt::PPMHeader header = crt::PPMHeader{imageWidth, imageHeight, maxColorCOmponent};

    std::ofstream ppmFile("task1_image.ppm", std::ios::out|std::ios::binary);
    if (ppmFile.fail()) {
        return -1;
    }

    header.setPPMFileHeader(ppmFile);

    drawImageWithRectangles(ppmFile, header);

    ppmFile.close();

    return 0;
}

static void drawImageWithRectangles(std::ofstream& ppmFile, const crt::PPMHeader& header) {
    // To get 4 rectangles on each row and column, we devide into intervals
    const int rectangles = 5;

    int intervalX = header.width / rectangles; 
    int intervalY = header.height / rectangles;

    const int arraySize = 6;

    std::array<crt::Color, arraySize> colorsArray = {
        crt::Color{header.maxColorComponent, 0, 0}, // Red
        crt::Color{0, header.maxColorComponent, 0}, // Green
        crt::Color{header.maxColorComponent, header.maxColorComponent, 0}, // Yellow
        crt::Color{0, 0, header.maxColorComponent}, // Blue
        crt::Color{header.maxColorComponent, 0, header.maxColorComponent}, // Purple
        crt::Color{0, header.maxColorComponent, header.maxColorComponent} // Cyan
    }; 

    int change = 1;
    crt::Color color;

    for (int currY = 0; currY < header.height; currY++) {
        if (currY % intervalY == 0) {
            change = (change + rectangles) % arraySize;
        }
        int idx = change;
        for (int currX = 0; currX < header.width; currX++) {
            if (currX % intervalX == 0) {
                color = colorsArray[idx];    
                idx = (idx + 1) % arraySize;
            }
            printColorToPPMFile(ppmFile, getRandomColor(color, header.maxColorComponent));
        }
        ppmFile << '\n';
    }
}

static crt::Color getRandomColor(crt::Color color, int maxColorComponent) {
    unsigned char r = rand() % (maxColorComponent + 1); 
    unsigned char g = rand() % (maxColorComponent + 1); 
    unsigned char b = rand() % (maxColorComponent + 1); 

    return crt::Color{
        std::max(r, color.r),
        std::max(g, color.g),
        std::max(b, color.b)
    }; 
}

static void printColorToPPMFile(std::ofstream& ppmFile, crt::Color color) {
    int r = static_cast<int>(color.r);
    int g = static_cast<int>(color.g);
    int b = static_cast<int>(color.b);

    ppmFile << r << ' ' << g << ' ' << b << '\t';
}