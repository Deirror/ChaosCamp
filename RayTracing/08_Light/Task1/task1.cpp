#include "RayTracer.h"

void task1() {
	crt::Scene scene("Scenes/scene3.crtscene");

	crt::RayTracer rayTracer(crt::RenderMode::Parallel);
	crt::ImageBuffer buffer = rayTracer.render(scene);

	buffer.save("task1_image.ppm", crt::PPMMode::P6);
}