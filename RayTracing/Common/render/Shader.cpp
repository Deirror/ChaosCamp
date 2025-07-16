#include "Shader.h"

#include "scene/Scene.h"

#include "Intersection.h"
#include "RayTracer.h"
#include "PathRay.h"
#include "Albedo.h"

CRT_BEGIN 

Shader::Shader(const Scene* scene, const RayTracer* rayTracer, unsigned short maxDepth)
	: scene_(scene), rayTracer_(rayTracer), maxDepth_(maxDepth) {
	CRT_ENSURE(scene != nullptr, "Scene is null pointer"); 
	CRT_ENSURE(rayTracer != nullptr, "RayTracer is null pointer"); 
}

Color Shader::shade(PathRay& pathRay, const HitRecord& hitRecord) const {
	if (maxDepth_ < pathRay.depth() || hitRecord.t >= FLT_MAX) {
		return fromVec3ToColor(scene_->settings().backgroundColor * 255.f);
	}
	++pathRay;

	Color finalColor;
	MaterialType materialType = scene_->material(hitRecord.materialIndex).materialType();

	switch (materialType) {
	case MaterialType::Diffuse:
		finalColor = shadeDiffuse(pathRay, hitRecord);
		break;
	case MaterialType::Reflective:
		finalColor = shadeReflective(pathRay, hitRecord);
		break;
	case MaterialType::Refractive:
		finalColor = shadeRefractive(pathRay, hitRecord);
		break;
	case MaterialType::Constant:
		finalColor = shadeConstant(pathRay, hitRecord);
		break;
	}
	--pathRay;

	return finalColor;
}

Color Shader::shadeDiffuse(PathRay& pathRay, const HitRecord& hitRecord) const {
	const Material& material = scene_->material(hitRecord.materialIndex);
	const Texture& texture = scene_->texture(scene_->textureIndex(material.albedoTextureName()));
	Vec3 albedo = sample(hitRecord);

	const Triangle triangle = scene_->triangle(hitRecord.triangleIndex);
	const Vec3& normal = material.smoothShading() ? hitRecord.hitNormal : triangle.normal();

	Vec3 accumulatedLight;

	if (scene_->lights().size()) {
		for (const auto& light : scene_->lights()) {
			Vec3 lightDir = light.position() - hitRecord.point;
			Vec3 lightDirNormalized = lightDir.normalized();

			float sphereRadius = lightDir.length();
			float sphereArea = math::PI4 * sphereRadius * sphereRadius;

			float cosLaw = math::max(0.f, dot(normal, lightDirNormalized));

			Ray shadowRay(hitRecord.point + normal * math::bias(dot(normal, lightDirNormalized)), lightDirNormalized, RayType::Shadow);

			HitRecord shadowHitRecord = rayTracer_->traceRayFunc()(shadowRay);

			float maxT = (light.position() - shadowRay.origin()).length();
			
			if (shadowHitRecord.t <= maxT) {
				const Material& shadowMat = scene_->material(shadowHitRecord.materialIndex);
				if (shadowMat.materialType() != MaterialType::Refractive) {
					continue;
				}
			}

			Vec3 lightContribution = (light.intensity() / sphereArea) * cosLaw * albedo;
			accumulatedLight += lightContribution;
		}
		accumulatedLight *= 255.f;
	}
	else {
		accumulatedLight = albedo * 255.f;
	}

	return fromVec3ToColor(accumulatedLight);
}

Color Shader::shadeReflective(PathRay& pathRay, const HitRecord& hitRecord) const {
	const Material& material = scene_->material(hitRecord.materialIndex);
	const Texture& texture = scene_->texture(scene_->textureIndex(material.albedoTextureName()));
	Vec3 albedo = sample(hitRecord);

	const Triangle triangle = scene_->triangle(hitRecord.triangleIndex);
	const Vec3& normal = material.smoothShading() ? hitRecord.hitNormal : triangle.normal();

	Vec3 projection = dot(pathRay.direction(), normal) * normal;
	Vec3 reflectDir = pathRay.direction() - 2.f * projection;

	Ray reflectRay(hitRecord.point + normal * math::bias(dot(normal, reflectDir)), reflectDir.normalized(), RayType::Reflective);

	pathRay.origin(reflectRay.origin());
	pathRay.direction(reflectRay.direction());
	pathRay.rayType(RayType::Reflective);

	HitRecord reflectHitRecord = rayTracer_->traceRayFunc()(pathRay);
	Vec3 color = fromColorToVec3(shade(pathRay, reflectHitRecord));

	return fromVec3ToColor(color * albedo);
}

Color Shader::shadeRefractive(PathRay& pathRay, const HitRecord& hitRecord) const { 
	const Material& material = scene_->material(hitRecord.materialIndex);
	const Texture& texture = scene_->texture(scene_->textureIndex(material.albedoTextureName()));
	Vec3 albedo = sample(hitRecord);
	const float materialIor = material.ior();

	const Triangle triangle = scene_->triangle(hitRecord.triangleIndex);
	Vec3 normal = material.smoothShading() ? hitRecord.hitNormal : triangle.normal();

	Vec3 incident = pathRay.direction().normalized();
	float cosIN = dot(incident, normal);

	float currIor = 1.f;
	float nextIor = materialIor;
	bool isLeaving = (cosIN > 0.f);

	if (isLeaving) {
		std::swap(currIor, nextIor); 
		normal *= -1.f;
	}

	Vec3 reflectionDir = incident - 2.0f * dot(incident, normal) * normal;
	Vec3 reflectionOrigin = hitRecord.point + normal * math::bias(dot(normal, reflectionDir));

	PathRay reflectionRay = pathRay;  
	reflectionRay.origin(reflectionOrigin);
	reflectionRay.direction(reflectionDir.normalized());
	reflectionRay.rayType(RayType::Reflective);
	reflectionRay.ior(currIor);  

	HitRecord reflectionHit = rayTracer_->traceRayFunc()(reflectionRay);
	Color reflectionColor = shade(reflectionRay, reflectionHit);

	cosIN = -dot(incident, normal);  
	float iorRatio = currIor / nextIor;
	float sin2RN = (1.f - cosIN * cosIN) * (iorRatio * iorRatio);

	if (sin2RN > 1.f) {
		return fromVec3ToColor(fromColorToVec3(reflectionColor) * albedo);
	}

	float sinRN = std::sqrtf(sin2RN);
	float cosRN = std::sqrtf(1.f - sin2RN);

	Vec3 refractProjN = cosRN * (normal * (-1.f));
	Vec3 parallel = (incident + cosIN * normal).normalized();
	Vec3 refractProjParallel = sinRN * parallel;

	Vec3 refractionDir = (refractProjN + refractProjParallel).normalized();
	Vec3 refractionOrigin = hitRecord.point - normal * math::bias(dot(normal * (-1.f), refractionDir));

	PathRay refractionRay = pathRay;  
	refractionRay.origin(refractionOrigin);
	refractionRay.direction(refractionDir);
	refractionRay.rayType(RayType::Refractive);
	refractionRay.ior(nextIor);  

	HitRecord refractionHit = rayTracer_->traceRayFunc()(refractionRay);
	Color refractionColor = shade(refractionRay, refractionHit);

	float R0 = (currIor - nextIor) / (currIor + nextIor);
	R0 = R0 * R0;

	float fresnel = R0 + (1.f - R0) * std::powf(1.f - cosIN, 5);

	return fromVec3ToColor(
		(fresnel * fromColorToVec3(reflectionColor) +
			(1.0f - fresnel) * fromColorToVec3(refractionColor)) * albedo
	);
}

Color Shader::shadeConstant(PathRay& pathRay, const HitRecord& hitRecord) const {
	return fromVec3ToColor(sample(hitRecord) * 255.f);
}

Vec3 Shader::sample(const HitRecord& hitRecord) const {
	const Texture& texture = scene_->texture(hitRecord.textureIndex);

	return albedo(texture, hitRecord);
}

CRT_END
