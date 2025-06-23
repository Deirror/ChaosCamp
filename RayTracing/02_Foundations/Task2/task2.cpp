#include <fstream>
#include <cmath>

#include "PPMFile.h"
#include "Resolution.h"
#include "Color.h"
#include "Vec3.h"

struct Circle {
    crt::Color color;
    crt::Vec3 center;
    int radius;
};

struct Lemniscate {
    crt::Color color;
    crt::Vec3 center;
    int a;
};

static void drawImageWithLemniscates(crt::PPMFile& file,
    const Lemniscate& lemn1, const Lemniscate& lemn2,
    const Circle& circle1, const Circle& circle2,
    const crt::Color& backColor);

void task2() {
    crt::Resolution resolution (1720, 1240);
    crt::PPMFile file("task2_image.ppm", resolution);

    crt::Vec3 lemnCenter(resolution.getWidth() / 2.0f, resolution.getHeight() / 2.0f, 0.0f);

    crt::Vec3 circle1Center(lemnCenter.x() + (lemnCenter.x() / 2), lemnCenter.y() - (lemnCenter.y() / 2), 0.0f);
    crt::Vec3 circle2Center(lemnCenter.x() - (lemnCenter.x() / 2), lemnCenter.y() + (lemnCenter.y() / 2), 0.0f);

    Lemniscate lemn1{ crt::Color{95, 0, 144}, lemnCenter, 300 };
    Lemniscate lemn2{ crt::Color{255, 204, 0}, lemnCenter, 300 };

    Circle circle1{ crt::Color{100, 0, 250}, circle1Center, 210 };
    Circle circle2{ crt::Color{100, 0, 250}, circle2Center, 210 };

    crt::Color backColor{ 0, 125, 44 };

    drawImageWithLemniscates(file, lemn1, lemn2, circle1, circle2, backColor);
}

static void drawImageWithLemniscates(crt::PPMFile& file,
    const Lemniscate& lemn1, const Lemniscate& lemn2,
    const Circle& circle1, const Circle& circle2,
    const crt::Color& backColor) {
    const crt::Resolution& resolution = file.getResolution();
    for (int y = 0; y < resolution.getHeight(); ++y) {
        for (int x = 0; x < resolution.getWidth(); ++x) {
            double dx = x - lemn1.center.x();
            double dy = y - lemn1.center.y();

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
                crt::Color blended{
                    (lemn1.color.r + lemn2.color.r) / 2.0f,
                    (lemn1.color.g + lemn2.color.g) / 2.0f,
                    (lemn1.color.b + lemn2.color.b) / 2.0f
                };
                file.printColor(blended);
            }
            else if (inFirst) {
                file.printColor(lemn1.color);
            }
            else if (inSecond) {
                file.printColor(lemn2.color);
                
            }
            else {
                int dist1 = std::sqrt(std::pow(x - circle1.center.x(), 2) + std::pow(y - circle1.center.y(), 2));
                int dist2 = std::sqrt(std::pow(x - circle2.center.x(), 2) + std::pow(y - circle2.center.y(), 2));

                if (dist1 <= circle1.radius) {
                    file.printColor(circle1.color);
                }
                else if (dist2 <= circle2.radius) {
                    file.printColor(circle2.color);
                }
                else {
                    file.printColor(backColor);
                }
            }
        }
        file.addNewLine();
    }
}
