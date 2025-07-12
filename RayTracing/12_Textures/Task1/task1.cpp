#include "render/RayTracer.h"
#include "image/ImageBuffer.h"

void task1() {
	crt::ParseOptions options;
	options.withOption(crt::JsonKey::MATERIALS).
		withOption(crt::JsonKey::LIGHTS).
		withOption(crt::JsonKey::TEXTURES);

	crt::Scene scene("Scenes/scene4.crtscene", options);

	crt::RayTracer rayTracer(&scene, crt::RenderMode::Parallel);
	crt::ImageBuffer buffer = rayTracer.render();

	buffer.save("task1_image.ppm", crt::PPMMode::P6);
}