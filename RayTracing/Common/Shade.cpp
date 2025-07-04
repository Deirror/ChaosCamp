#include "Shade.h"

CRT_BEGIN 

Color shade(const HitRecord& hitRecord, const Scene& scene) {
	Vec3 albedo(0.5f, 0.1f, 0.6f);
	Vec3 accumulatedLight(0.f, 0.f, 0.f);

	for (const auto& light : scene.lights()) {
		Vec3 lightDir = light.position() - hitRecord.point;
		Vec3 lightDirNormalized = lightDir.normalized();

		float sphereRadius = lightDir.length();
		float sphereArea = 4.f * math::PI * sphereRadius * sphereRadius;

		float cosLaw = math::max(0.f, dot(hitRecord.normal, lightDirNormalized));

		float bias = math::EPSILON_RAY + 1e-3 * (1 - dot(hitRecord.normal, lightDirNormalized));
		Ray shadowRay(hitRecord.point + hitRecord.normal * bias, lightDirNormalized);
		bool isShadowed = false;
		HitRecord shadowHit;

		for (const auto& tri : scene.triangles()) {
			if (&tri == hitRecord.triangle) continue;

			if (tri.intersect(shadowRay, shadowHit, false)) {
				if (shadowHit.t < sphereRadius - math::EPSILON_ZERO) {
					isShadowed = true;
					break;
				}
			}
		}

		if (isShadowed) continue;

		Vec3 lightContribution = (light.intensity() / sphereArea) * cosLaw * albedo;
		accumulatedLight += lightContribution;
	}

	Color finalColor;

	finalColor.r = static_cast<unsigned char>(math::clamp(accumulatedLight.x() * 255.f, 0.f, 255.f));
	finalColor.g = static_cast<unsigned char>(math::clamp(accumulatedLight.y() * 255.f, 0.f, 255.f));
	finalColor.b = static_cast<unsigned char>(math::clamp(accumulatedLight.z() * 255.f, 0.f, 255.f));

	return finalColor;
}

CRT_END
