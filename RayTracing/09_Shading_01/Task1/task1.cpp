#include "RayTracer.h"
#include "ImageBuffer.h"

void task1() {
	crt::ParseOptions options;
	options.withOption(crt::JsonKey::MATERIALS);

	crt::Scene scene("Scenes/scene1.crtscene", options);

	crt::RayTracer rayTracer(crt::RenderMode::Parallel);
	crt::ImageBuffer buffer = rayTracer.render(scene);

	buffer.save("task1_image.ppm", crt::PPMMode::P6);
}