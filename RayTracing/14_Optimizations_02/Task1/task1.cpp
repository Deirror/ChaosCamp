#include "render/RayTracer.h"
#include "image/ImageBuffer.h"
#include "core/profile/Timer.h"

void task1() {
	crt::ParseOptions options;
	options.withOption(crt::JsonKey::MATERIALS)
		.withOption(crt::JsonKey::LIGHTS)
		.withOption(crt::JsonKey::TEXTURES);

	crt::Scene scene("Scenes/walle.crtscene", options);

	crt::RayTracer rayTracer(&scene, crt::RenderMode::Buckets, crt::AccelerationType::KDTree);
	crt::ImageBuffer buffer;

	{
		Timer timer("render_profile.log", "WALL-E - KDTree");
		buffer = rayTracer.render();
	}

	buffer.save("task1_image.ppm", crt::PPMMode::P6);
}