#include "RayTracer.h"
#include "ImageBuffer.h"

void task1() {
	crt::Scene scene("Scenes/scene2.crtscene");

	crt::RayTracer rayTracer(crt::RenderMode::Parallel);
	crt::ImageBuffer buffer = rayTracer.render(scene);

	buffer.save("task1_image.ppm", crt::PPMMode::P6);
}
