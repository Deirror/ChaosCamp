#include <fstream>
#include <cmath>

#include "Common/ppm.h"

struct Vec3 {
    int x, y, z;
};

struct Circle {
    Vec3 color;
    Vec3 center;
    int radius;
};

struct Lemniscate {
    Vec3 color;
    Vec3 center;
    int a;
};

static void drawImageWithLemniscates(std::ofstream& ppmFile, const crt::PPMHeader& header,
                                     const Lemniscate& lemn1, const Lemniscate& lemn2,
                                     const Circle& circle1, const Circle& circle2,
                                     const Vec3& backColor);
static void printColorToPPMFile(std::ofstream&, const Vec3&);

int main() {
    const int imageWidth = 1720;
    const int imageHeight = 1240;
    const int maxColorComponent = 255;

    crt::PPMHeader header{imageWidth, imageHeight, maxColorComponent};
    std::ofstream ppmFile("task2_image.ppm", std::ios::out | std::ios::binary);
    if (ppmFile.fail()) return -1;

    header.setPPMFileHeader(ppmFile);

    Vec3 lemnCenter{imageWidth / 2, imageHeight / 2};

    Vec3 circle1Center{ lemnCenter.x + (lemnCenter.x / 2), lemnCenter.y - (lemnCenter.y / 2)};
    Vec3 circle2Center{ lemnCenter.x - (lemnCenter.x / 2), lemnCenter.y + (lemnCenter.y / 2)};

    Lemniscate lemn1{Vec3{95, 0, 144}, lemnCenter, 300}; 
    Lemniscate lemn2{Vec3{255, 204, 0}, lemnCenter, 300}; 

    Circle circle1{Vec3{100, 0, 250}, circle1Center, 210};
    Circle circle2{Vec3{100, 0, 250}, circle2Center, 210};

    Vec3 backColor{0, 125, 44};

    drawImageWithLemniscates(ppmFile, header, lemn1, lemn2, circle1, circle2, backColor);

    ppmFile.close();
    return 0;
}

static void setPPMFileHeader(std::ofstream& ppmFile, const crt::PPMHeader& header) {
    ppmFile << "P3\n";
    ppmFile << header.width << " " << header.height << '\n'; 
    ppmFile << header.maxColorComponent << '\n';
}

static void drawImageWithLemniscates(std::ofstream& ppmFile, const crt::PPMHeader& header,
                                     const Lemniscate& lemn1, const Lemniscate& lemn2,
                                     const Circle& circle1, const Circle& circle2,
                                     const Vec3& backColor) {
    for (int y = 0; y < header.height; ++y) {
        for (int x = 0; x < header.width; ++x) {
            double dx = x - lemn1.center.x;
            double dy = y - lemn1.center.y;

            double r2 = dx * dx + dy * dy;
            double theta = std::atan2(dy, dx);

            bool inFirst = false;
            bool inSecond = false;

            double cos2t = std::cos(2 * theta);
            if (cos2t >= 0) {
                double r2max1 = lemn1.a * lemn1.a * cos2t;
                if (r2 <= r2max1) inFirst = true;
            }

            double sin2t = std::sin(2 * theta);
            if (sin2t >= 0) {
                double r2max2 = lemn2.a * lemn2.a * sin2t;
                if (r2 <= r2max2) inSecond = true;
            }

            if (inFirst && inSecond) {
                Vec3 blended{
                    (lemn1.color.x + lemn2.color.x) / 2,
                    (lemn1.color.y + lemn2.color.y) / 2,
                    (lemn1.color.z + lemn2.color.z) / 2
                };
                printColorToPPMFile(ppmFile, blended);
            } else if (inFirst) {
                printColorToPPMFile(ppmFile, lemn1.color);
            } else if (inSecond) {
                printColorToPPMFile(ppmFile, lemn2.color);
            } else {
                int dist1 = std::sqrt(std::pow(x - circle1.center.x,2) + std::pow(y - circle1.center.y,2));
                int dist2 = std::sqrt(std::pow(x - circle2.center.x,2) + std::pow(y - circle2.center.y,2));

                if (dist1 <= circle1.radius) {
                    printColorToPPMFile(ppmFile, circle1.color);
                } else if (dist2 <= circle2.radius) {
                    printColorToPPMFile(ppmFile, circle2.color);
                } else {
                    printColorToPPMFile(ppmFile, backColor);
                }
            }
        }
        ppmFile << '\n';
    }
}

static void printColorToPPMFile(std::ofstream& ppmFile, const Vec3& color) {
    ppmFile << color.x << ' ' << color.y << ' ' << color.z << '\t';
}
