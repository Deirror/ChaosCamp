#include "render/Animation.h"
#include "image/ImageBuffer.h"

namespace WALLE {

	void walle() {
		crt::ParseOptions options;
		options.withOption(crt::JsonKey::LIGHTS);

		crt::Scene scene("Scenes/walle.crtscene", options);

		crt::RayTracer rayTracer(&scene, crt::RayTracer::RenderMode::Buckets, crt::RayTracer::AccelerationType::KDTree);
		crt::ImageBuffer buffer = rayTracer.render();

		buffer.save("walle.ppm", crt::PPMWriter::Format::P6);
	}

	void walle_animation() {
		crt::ParseOptions options;
		options.withOption(crt::JsonKey::LIGHTS);

		crt::Scene scene("Scenes/walle.crtscene", options);

		crt::RayTracer rayTracer(&scene, crt::RayTracer::RenderMode::Buckets, crt::RayTracer::AccelerationType::KDTree);
		crt::Animation animation(rayTracer);

		animation.zigZagOrbit(crt::math::radians(-0.51f), crt::math::radians(0.02f), 181, 361);
	}
}