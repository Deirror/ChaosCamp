#include "Shade.h"

CRT_BEGIN 

Color shade(const HitRecord& hitRecord, const Scene& scene) {
	unsigned int materialIndex = scene.meshes()[hitRecord.meshIndex].materialIndex();
	Material material = scene.materials()[materialIndex];
	Color albedo = material.albedo();
	Vec3 albedoVec = { albedo.r / 255.f, albedo.g / 255.f, albedo.b / 255.f };
	Vec3 accumulatedLight(0.f, 0.f, 0.f);

	const Triangle triangle = scene.triangle(hitRecord.triangleIndex);
	Vec3 normal = triangle.normal();
	if (material.smoothShading()) {
		normal = hitRecord.normal;
	}

	for (const auto& light : scene.lights()) {
		Vec3 lightDir = light.position() - hitRecord.point;
		Vec3 lightDirNormalized = lightDir.normalized();

		float sphereRadius = lightDir.length();
		float sphereArea = math::PI4 * sphereRadius * sphereRadius;

		float cosLaw = math::max(0.f, dot(normal, lightDirNormalized));

		float bias = math::EPSILON_RAY + math::SLOPE_BIAS * (1.f - dot(normal, lightDirNormalized));
		Ray shadowRay(hitRecord.point + normal * bias, lightDirNormalized);
		bool isShadowed = false;
		HitRecord shadowHit;

		for (const auto& sceneTriangle : scene.triangles()) {
			const auto& tri = scene.triangle(sceneTriangle.triangleIndex);
			if (&tri == &triangle) continue;

			if (tri.intersect(shadowRay, shadowHit, false)) {
				if (shadowHit.t < sphereRadius - math::EPSILON_ZERO) {
					isShadowed = true;
					break;
				}
			}
		}

		if (isShadowed) continue;

		Vec3 lightContribution = (light.intensity() / sphereArea) * cosLaw * albedoVec;
		accumulatedLight += lightContribution;
	}

	Color finalColor;

	finalColor.r = static_cast<unsigned char>(math::clamp(accumulatedLight.x() * 255.f, 0.f, 255.f));
	finalColor.g = static_cast<unsigned char>(math::clamp(accumulatedLight.y() * 255.f, 0.f, 255.f));
	finalColor.b = static_cast<unsigned char>(math::clamp(accumulatedLight.z() * 255.f, 0.f, 255.f));

	return finalColor;
}

CRT_END
