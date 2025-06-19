#include <fstream>
#include <random>
#include <array>

#include <iostream>

struct PPMHeader {
    int width;
    int height;
    unsigned char maxColorComponent; // max can be 255 so it fits well in unsigned char
};

struct Color {
    unsigned char r, g, b;
};

static void setPPMFileHeader(std::ofstream&, const PPMHeader&);

static void drawImageWithRectangles(std::ofstream&, const PPMHeader&);
static Color getRandomColor(Color, int); 
static void printColorToPPMFile(std::ofstream&, Color);

static Color getRandomColor(Color, int&);

int main() {
    const int imageWidth = 1720,
            imageHeight = 1240,
            maxColorCOmponent = 255;

    PPMHeader header = PPMHeader{imageWidth, imageHeight, maxColorCOmponent};

    std::ofstream ppmFile("task1_image.ppm", std::ios::out|std::ios::binary);
    if (ppmFile.fail()) {
        return -1;
    }

    setPPMFileHeader(ppmFile, header);

    drawImageWithRectangles(ppmFile, header);

    ppmFile.close();

    return 0;
}

static void setPPMFileHeader(std::ofstream& ppmFile, const PPMHeader& header) {
    ppmFile << "P3\n";
    ppmFile << header.width << " " << header.height << '\n'; 
    ppmFile << static_cast<int>(header.maxColorComponent) << '\n';
}

static void drawImageWithRectangles(std::ofstream& ppmFile, const PPMHeader& header) {
    // To get 4 rectangles on each row and column, we devide into intervals
    const int rectangles = 5;

    int intervalX = header.width / rectangles; 
    int intervalY = header.height / rectangles;

    const int arraySize = 6;

    std::array<Color, arraySize> colorsArray = {
        Color{header.maxColorComponent, 0, 0}, // Red
        Color{0, header.maxColorComponent, 0}, // Green
        Color{header.maxColorComponent, header.maxColorComponent, 0}, // Yellow
        Color{0, 0, header.maxColorComponent}, // Blue
        Color{header.maxColorComponent, 0, header.maxColorComponent}, // Purple
        Color{0, header.maxColorComponent, header.maxColorComponent} // Cyan
    }; 

    int change = 1;
    Color color;

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

static Color getRandomColor(Color color, int maxColorComponent) {
    unsigned char r = rand() % (maxColorComponent + 1); 
    unsigned char g = rand() % (maxColorComponent + 1); 
    unsigned char b = rand() % (maxColorComponent + 1); 

    return Color{
        std::max(r, color.r),
        std::max(g, color.g),
        std::max(b, color.b)
    }; 
}

static void printColorToPPMFile(std::ofstream& ppmFile, Color color) {
    int r = static_cast<int>(color.r);
    int g = static_cast<int>(color.g);
    int b = static_cast<int>(color.b);

    ppmFile << r << ' ' << g << ' ' << b << '\t';
}