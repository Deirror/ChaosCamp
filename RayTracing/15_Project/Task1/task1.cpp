#include "render/Animation.h"
#include "image/ImageBuffer.h"

namespace task1 {

	void zebra_dragon_room_animation() {
		crt::ParseOptions options;
		options.withOption(crt::JsonKey::LIGHTS);

		crt::Scene scene("Scenes/zebra_dragon_room.crtscene", options);

		crt::RayTracer rayTracer(&scene, crt::RayTracer::RenderMode::Buckets, crt::RayTracer::AccelerationType::KDTree);
		crt::Animation animation(rayTracer);

		scene.camera().orbit(crt::math::radians(0.1f), 0);

		animation.vertigo(-0.3f, crt::math::radians(-1.8f), 30);
		animation.staticSequence(5);
		animation.vertigo(0.3f, crt::math::radians(1.8f), 30);
		animation.staticSequence(10);

		animation.craneReveal(-0.20f, 0.05f, crt::math::radians(0.5f), {0, 0, -1}, 40);

		animation.zigZagOrbit(crt::math::radians(2.f), crt::math::radians(-1.f), 18, 180);
	}

	void zebra_dragom_room() {
		crt::ParseOptions options;
		options.withOption(crt::JsonKey::LIGHTS);

		crt::Scene scene("Scenes/zebra_dragon_room.crtscene", options);

		crt::RayTracer rayTracer(&scene, crt::RayTracer::RenderMode::Buckets, crt::RayTracer::AccelerationType::KDTree);
		crt::ImageBuffer buffer = rayTracer.render();

		buffer.save("zebra_dragon_room.ppm", crt::PPMWriter::Format::P6);
	}

	void gi_room() {
		crt::ParseOptions options;
		options.withOption(crt::JsonKey::LIGHTS);

		crt::Scene scene("Scenes/gi_room.crtscene", options);

		crt::RayTracer rayTracer(&scene, crt::RayTracer::RenderMode::Buckets, crt::RayTracer::RayTracer::AccelerationType::KDTree);
		crt::ImageBuffer buffer = rayTracer.render();

		buffer.save("gi_room.ppm", crt::PPMWriter::Format::P6);
	}

	void gi_room_sphere() {
		crt::ParseOptions options;
		options.withOption(crt::JsonKey::LIGHTS);

		crt::Scene scene("Scenes/gi_room_sphere.crtscene", options);

		crt::RayTracer rayTracer(&scene, crt::RayTracer::RenderMode::Buckets, crt::RayTracer::RayTracer::AccelerationType::KDTree);
		crt::Animation animation(rayTracer);

		animation.orbit(crt::math::radians(2.f), 0.f, 180);
	}

	void disco_sphere_animation() {
		crt::ParseOptions options;
		options.withOption(crt::JsonKey::LIGHTS);

		crt::Scene scene("Scenes/sphere.crtscene", options);

		crt::RayTracer rayTracer(&scene, crt::RayTracer::RenderMode::Buckets, crt::RayTracer::AccelerationType::KDTree);
		crt::Animation animation(rayTracer);

		animation.modifyLighting(0.2f, 30);
		animation.modifyLighting(0.3f, 30);
		
		animation.staticSequence(10);

		animation.disco(0.5f, {1}, 160);

		animation.staticSequence(10);

		animation.modifyLighting(-0.34f, 40);
	}
}