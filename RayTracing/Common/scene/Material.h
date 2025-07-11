#pragma once

#include <string>

#include "core/error.h"
#include "image/Color.h"

CRT_BEGIN

enum class MaterialType {
	Air,
	Diffuse,
	Reflective,
	Refractive,
	Constant
};

MaterialType fromStringToMaterialType(const std::string& type);

class Material {
public:
	Material() = default;
	Material(MaterialType materialType, const Vec3& albedo, bool smoothShading = false, float ior = 1.f)
		: materialType_(materialType), albedo_(albedo), smoothShading_(smoothShading), ior_(ior) {}

	MaterialType materialType() const { return materialType_; }
	void materialType(MaterialType materialType) { materialType_ = materialType; }

	const Vec3& albedo() const { return albedo_; }
	void albedo(const Vec3& albedo) { albedo_ = albedo; }
	
	bool smoothShading() const { return smoothShading_; }
	void smoothShading(bool smoothShading) { smoothShading_ = smoothShading; }

	float ior() const { return ior_; }
	void ior(float ior) { ior_ = ior; }

private:
	MaterialType materialType_ = MaterialType::Air;
	Vec3 albedo_ = Vec3(1.f, 1.f, 1.f);
	bool smoothShading_ = false;

	float ior_ = 1.f;
};

CRT_END
