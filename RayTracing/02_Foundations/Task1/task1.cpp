#include <fstream>
#include <random>
#include <array>

#include <iostream>

#include "PPMFile.h"
#include "Color.h"
#include "Resolution.h"

static void drawImageWithRectangles(crt::PPMFile& file);
static crt::Color getSpecialRandomColor(crt::Color color, int maxColorComponent);

void task1() {
    crt::Resolution resolution = crt::Resolution{ 1720, 1240 };
    crt::PPMFile file("task1_image.ppm", resolution);

    drawImageWithRectangles(file);
}

static void drawImageWithRectangles(crt::PPMFile& file) {
    // To get 5 rectangles on each row and column, we devide into intervals
    const int rectangles = 5;
    const crt::Resolution& resolution = file.getResolution();

    int intervalX = resolution.getHeight() / rectangles;
    int intervalY = resolution.getHeight() / rectangles;

    const int arraySize = 6;

    std::array<crt::Color, arraySize> colorsArray = {
        crt::Color{file.getMaxColorComponent(), 0, 0}, // Red
        crt::Color{0, file.getMaxColorComponent(), 0}, // Green
        crt::Color{file.getMaxColorComponent(), file.getMaxColorComponent(), 0}, // Yellow
        crt::Color{0, 0, file.getMaxColorComponent()}, // Blue
        crt::Color{file.getMaxColorComponent(), 0, file.getMaxColorComponent()}, // Purple
        crt::Color{0, file.getMaxColorComponent(), file.getMaxColorComponent()} // Cyan
    };

    int change = 1;
    crt::Color color;

    for (int currY = 0; currY < resolution.getHeight(); currY++) {
        if (currY % intervalY == 0) {
            change = (change + rectangles) % arraySize;
        }
        int idx = change;
        for (int currX = 0; currX < resolution.getWidth(); currX++) {
            if (currX % intervalX == 0) {
                color = colorsArray[idx];
                idx = (idx + 1) % arraySize;
            }
            file.printColor(getSpecialRandomColor(color, file.getMaxColorComponent()));
        }
        file.addNewLine();
    }
}

static crt::Color getSpecialRandomColor(crt::Color color, int maxColorComponent) {
    unsigned char r = rand() % (maxColorComponent + 1);
    unsigned char g = rand() % (maxColorComponent + 1);
    unsigned char b = rand() % (maxColorComponent + 1);

    return crt::Color{
        std::max(r, color.r),
        std::max(g, color.g),
        std::max(b, color.b)
    };
}
