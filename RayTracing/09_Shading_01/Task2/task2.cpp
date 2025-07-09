#include "RayTracer.h"
#include "ImageBuffer.h"

void task2() {
	crt::ParseOptions options;
	options.withOption(crt::JsonKey::MATERIALS)
		.withOption(crt::JsonKey::LIGHTS);

	crt::Scene scene("Scenes/scene3.crtscene", options);

	crt::RayTracer rayTracer(crt::RenderMode::Parallel);
	crt::ImageBuffer buffer = rayTracer.render(scene);

	buffer.save("task2_image.ppm", crt::PPMMode::P6);
}