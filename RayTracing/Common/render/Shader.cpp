#include "Shader.h"

#include "scene/Scene.h"

#include "Intersection.h"
#include "RayTracer.h"
#include "PathRay.h"

CRT_BEGIN 

static Vec3 directionToUV(const Vec3& dir) {
	float u = 0.5f + (std::atan2(dir.z(), dir.x()) / (2.f * math::PI));
	float v = 1.f - (acos(math::clamp(dir.y(), -1.f, 1.f)) / math::PI);
	return Vec3(u, v, 0.f);
}

Shader::Shader(const Scene* scene, const RayTracer* rayTracer) 
	: scene_(scene), rayTracer_(rayTracer) {
	CRT_ENSURE(scene != nullptr, "Scene is null pointer"); 
	CRT_ENSURE(rayTracer != nullptr, "RayTracer is null pointer"); 

	updateRayParams();
}

Vec3 Shader::shade(PathRay& pathRay, const HitRecord& hitRecord) const {
	if (maxDepth_ <= pathRay.depth() || hitRecord.t >= FLT_MAX) {
		unsigned int textureIndex = scene_->textureIndex(scene_->settings().backgroundAlbedoTextureName);

		HitRecord backgroundHit;
		backgroundHit.textureIndex = textureIndex;

		if (scene_->texture(textureIndex).textureType() != TextureType::Albedo) {
			backgroundHit.puv = directionToUV(pathRay.direction());
		}

		return sample(backgroundHit);
	}
	++pathRay;

	Vec3 finalColor;
	MaterialType materialType = scene_->material(hitRecord.materialIndex).materialType();

	switch (materialType) {
	case MaterialType::Diffuse:
		finalColor = shadeDiffuse(pathRay, hitRecord);
		break;
	case MaterialType::Reflective:
		if (rayEffects.reflection == true) {
			finalColor = shadeReflective(pathRay, hitRecord);
		}
		break;
	case MaterialType::Refractive:
		if (rayEffects.refraction == true) {
			finalColor = shadeRefractive(pathRay, hitRecord);
		}
		break;
	case MaterialType::Constant:
		finalColor = shadeConstant(pathRay, hitRecord);
		break;
	}
	--pathRay;

	return finalColor;
}

void Shader::scene(const Scene* scene) { 
	CRT_ENSURE(scene != nullptr, "Scene is null pointer"); 
	scene_ = scene; 
	updateRayParams();
}

Vec3 Shader::sampleRandomHemisphere() const {
	float randRadsXY = math::PI * math::randf();
	Vec3 randVecXY(std::cos(randRadsXY), std::sin(randRadsXY), 0.f);

	float randRadsXZ = 2 * math::PI * math::randf();
	Mat3 matAroundY(
		{ std::cos(randRadsXZ), 0.f, -std::sin(randRadsXZ) },
		{ 0.f, 1.f, 0.f },
		{ std::sin(randRadsXZ), 0, std::cos(randRadsXZ) }
	);

	randVecXY = matAroundY * randVecXY;

	return randVecXY;
}

void Shader::updateRayParams() {
	RaySettings raySettings = scene_->settings().raySettings;

	giRays_ = raySettings.giRays;
	maxDepth_ = raySettings.maxDepth;

	rayEffects.reflection = raySettings.reflection;
	rayEffects.refraction = raySettings.refraction;
}

Vec3 Shader::shadeDiffuse(PathRay& pathRay, const HitRecord& hitRecord) const {
	Vec3 diColor = shadeDirectIllumination(pathRay, hitRecord);
	if (giRays_ > 0) {
		Vec3 giColor = shadeGlobalIllumination(pathRay, hitRecord);
		return (diColor + giColor) / (float)(giRays_);
	}
	return diColor;
}

Vec3 Shader::shadeDirectIllumination(PathRay& pathRay, const HitRecord& hitRecord) const {
	const Material& material = scene_->material(hitRecord.materialIndex);
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

			SceneRay shadowRay(hitRecord.point + normal * math::bias(dot(normal, lightDirNormalized)), lightDirNormalized, RayType::Shadow);

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
	}
	else {
		accumulatedLight = albedo;
	}

	return accumulatedLight;
}

Vec3 Shader::shadeGlobalIllumination(PathRay& pathRay, const HitRecord& hitRecord) const {
	const Material& material = scene_->material(hitRecord.materialIndex);

	const Triangle& triangle = scene_->triangle(hitRecord.triangleIndex);
	const Vec3& normal = material.smoothShading() ? hitRecord.hitNormal : triangle.normal();

	Vec3 right = cross(pathRay.direction(), normal).normalized();
	Vec3 forward = cross(right, normal).normalized();

	Mat3 localHitMat(right, normal, forward);

	Vec3 accumulatedLight;
	for (int count = 0; count < giRays_; ++count) {
		Vec3 reflectDir = localHitMat * sampleRandomHemisphere();
	
		SceneRay reflectRay(hitRecord.point + normal * math::bias(dot(normal, reflectDir)), reflectDir, RayType::Reflective);

		PathRay reflectPathRay(reflectRay);
		reflectPathRay.depth(pathRay.depth());

		HitRecord reflectHitRecord = rayTracer_->traceRayFunc()(reflectPathRay);
		accumulatedLight += shade(reflectPathRay, reflectHitRecord);
	}

	return Vec3::clamp(accumulatedLight);
}

Vec3 Shader::shadeReflective(PathRay& pathRay, const HitRecord& hitRecord) const {
	const Material& material = scene_->material(hitRecord.materialIndex);
	Vec3 albedo = sample(hitRecord);

	const Triangle& triangle = scene_->triangle(hitRecord.triangleIndex);
	const Vec3& normal = material.smoothShading() ? hitRecord.hitNormal : triangle.normal();

	Vec3 projection = dot(pathRay.direction(), normal) * normal;
	Vec3 reflectDir = pathRay.direction() - 2.f * projection;

	SceneRay reflectRay(hitRecord.point + normal * math::bias(dot(normal, reflectDir)), reflectDir.normalized(), RayType::Reflective);

	pathRay.origin(reflectRay.origin());
	pathRay.direction(reflectRay.direction());
	pathRay.type(reflectRay.type());

	HitRecord reflectHitRecord = rayTracer_->traceRayFunc()(pathRay);
	Vec3 color = shade(pathRay, reflectHitRecord);

	return color * albedo;
}

Vec3 Shader::shadeRefractive(PathRay& pathRay, const HitRecord& hitRecord) const { 
	const Material& material = scene_->material(hitRecord.materialIndex);
	Vec3 albedo = sample(hitRecord);
	const float materialIor = material.ior();

	const Triangle& triangle = scene_->triangle(hitRecord.triangleIndex);
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

	Vec3 reflectionDir = incident - 2.f * dot(incident, normal) * normal;
	Vec3 reflectionOrigin = hitRecord.point + normal * math::bias(dot(normal, reflectionDir));

	PathRay reflectionRay = pathRay;  
	reflectionRay.origin(reflectionOrigin);
	reflectionRay.direction(reflectionDir.normalized());
	reflectionRay.type(RayType::Reflective);
	reflectionRay.ior(currIor);  

	HitRecord reflectionHit = rayTracer_->traceRayFunc()(reflectionRay);
	Vec3 reflectionColor = shade(reflectionRay, reflectionHit);

	cosIN = -dot(incident, normal);  
	float iorRatio = currIor / nextIor;
	float sin2RN = (1.f - cosIN * cosIN) * (iorRatio * iorRatio);

	if (sin2RN > 1.f) {
		return reflectionColor * albedo;
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
	refractionRay.type(RayType::Refractive);
	refractionRay.ior(nextIor);  

	HitRecord refractionHit = rayTracer_->traceRayFunc()(refractionRay);
	Vec3 refractionColor = shade(refractionRay, refractionHit);

	float R0 = (currIor - nextIor) / (currIor + nextIor);
	R0 = R0 * R0;

	float fresnel = R0 + (1.f - R0) * std::powf(1.f - cosIN, 5);

	return (fresnel * (reflectionColor) + (1.f - fresnel) * (refractionColor)) * albedo;
}

Vec3 Shader::shadeConstant(PathRay& pathRay, const HitRecord& hitRecord) const {
	return sample(hitRecord);
}

Vec3 Shader::sample(const HitRecord& hitRecord) const {
	const Texture& texture = scene_->texture(hitRecord.textureIndex);

	switch (texture.textureType()) {
	case TextureType::Albedo: {
		return texture.albedo();
	}
	case TextureType::Edges: {
		return texture.albedo(hitRecord.barycentricCoords);
	}
	case TextureType::Zebra:
	case TextureType::Bitmap:
	case TextureType::Checker: {
		return texture.albedo(hitRecord.puv);
	}
	}

	CRT_ERROR("Unknown texture type passed");
}

CRT_END
