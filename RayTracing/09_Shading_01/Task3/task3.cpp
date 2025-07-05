#include "Render.h"
#include "ImageBuffer.h"

void task3() {
	crt::ParseOptions options;
	options.withOption(crt::JsonKey::MATERIALS)
		.withOption(crt::JsonKey::LIGHTS);

	crt::Scene scene("Scenes/scene5.crtscene", options);

	crt::Render render(crt::RenderMode::Parallel);
	crt::ImageBuffer buffer = render.render(scene);

	buffer.save("task2_image.ppm", crt::PPMMode::P6);
}