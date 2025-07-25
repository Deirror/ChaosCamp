#include "Animation.h"

#include <format>

CRT_BEGIN

void Animation::staticSequence(unsigned int frames) {
	for (unsigned int i = 0; i < frames; ++i) {
		if (!skipSaveFrame_) {
			saveFrame();
		}
		++frame_;
	}
}

void Animation::craneReveal(float dz, float dy, float tilt, const Vec3& lookAt, unsigned int frames) {
	Camera& camera = rayTracer_.scene()->camera();

	for (unsigned int i = 0; i < frames; ++i) {
		if (!skipSaveFrame_) {
			saveFrame();
		}
		++frame_;
		
		camera.dolly(dz);
		camera.pedestal(dy);
		camera.tilt(tilt);
		camera.lookAt(lookAt);
	}
}

void Animation::orbit(float pan, float tilt, unsigned int frames) {
	Camera& camera = rayTracer_.scene()->camera();

	for (unsigned int i = 0; i < frames; ++i) {
		if (!skipSaveFrame_) {
			saveFrame();
		}
		++frame_;
		
		camera.orbit(pan, tilt);
	}
}

void Animation::zigZagOrbit(float pan, float tilt, unsigned int steps, unsigned int frames) {
	Camera& camera = rayTracer_.scene()->camera();

	for (unsigned int i = 0; i < frames; ++i) {
		if (!skipSaveFrame_) {
			saveFrame();
		}
		++frame_;

		if ((i + 1) % steps == 0) {
			tilt *= -1.f;
		}
		
		camera.orbit(pan, tilt);
	}
}

void Animation::vertigo(float dz, float fov, unsigned int frames) {
	Camera& camera = rayTracer_.scene()->camera();
	float cameraFov = camera.fov();

	for (unsigned int i = 0; i < frames; ++i) {
		if (!skipSaveFrame_) {
			saveFrame();
		}
		++frame_;

		camera.dolly(dz);
		cameraFov += fov;
		camera.fov(cameraFov);
	}
}

void Animation::modifyLighting(float intensity, const std::vector<unsigned int>& indexes, unsigned int frames) {
	Scene* scene = rayTracer_.scene();

	for (unsigned int i = 0; i < frames; ++i) {
		if (!skipSaveFrame_) {
			saveFrame();
		}
		++frame_;

		for (auto index : indexes) {
			Light& light = scene->light(index);

			float lightIntensity = light.intensity();
			light.intensity(lightIntensity + intensity);
		}
	}
}

void Animation::modifyLighting(float intensity, unsigned int frames) {
	Scene* scene = rayTracer_.scene();
	size_t lightCount = scene->lights().size();

	for (unsigned int i = 0; i < frames; ++i) {
		if (!skipSaveFrame_) {
			saveFrame();
		}
		++frame_;

		for (unsigned int index = 0; index < lightCount; ++index) {
			Light& light = scene->light(index);

			float lightIntensity = light.intensity();
			light.intensity(lightIntensity + intensity);
		}
	}
}

void Animation::disco(float speed, const std::vector<unsigned int>& meshIndexes, unsigned int frames) {
	Scene* scene = rayTracer_.scene();

	speed = math::clamp(speed, 0.f, 0.5f);

	const float minValue = 0.5f;
	const float maxValue = 1.0f;
	float current = minValue;

	int direction = 1;      
	int channel = 2;       
	float step = speed * 0.05f;  

	for (unsigned int i = 0; i < frames; ++i) {
		Vec3 color(minValue, minValue, minValue);
		color[channel] = current;

		for (auto index : meshIndexes) {
			const Mesh& mesh = scene->mesh(index);
			const Material& material = scene->material(mesh.materialIndex());

			unsigned int texIndex = scene->textureIndex(material.albedoTextureName());
			Texture& texture = scene->texture(texIndex);

			AlbedoTextureData data(color);
			texture.albedoTextureData(data);
		}

		if (!skipSaveFrame_) {
			saveFrame();
		}
		++frame_;

		current += direction * step;
		if (current >= maxValue) {
			current = maxValue;
			direction = -1; 
		}
		else if (current <= minValue) {
			current = minValue;
			direction = 1; 
			channel = (channel + 2) % 3;
		}
	}
}

void Animation::saveFrame() const {
	crt::ImageBuffer buffer = rayTracer_.render();

	std::string filename = std::format("{}_{}.ppm", output_, frame_);
	buffer.save(filename, PPMWriter::Format::P6);
}

CRT_END

