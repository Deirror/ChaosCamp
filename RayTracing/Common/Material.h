#pragma once

#include <string>

#include "error.h"
#include "Color.h"

CRT_BEGIN

enum class MaterialType {
	DIFFUSE,
	REFLECTIVE,
	REFRACTIVE,
};

MaterialType fromStringToMaterialType(const std::string& type);

class Material {
public:
	Material() = default;
	Material(MaterialType materialType, const Color& albedo, bool smoothShading)
		: materialType_(materialType), albedo_(albedo), smoothShading_(smoothShading) {}

	MaterialType materialType() const { return materialType_; }
	const Color& albedo() const { return albedo_; }
	bool smoothShading() const { return smoothShading_; }

	void materialType(MaterialType materialType) { materialType_ = materialType; }
	void albedo(const Color& albedo) { albedo_ = albedo; }
	void smoothShading(bool smoothShading) { smoothShading_ = smoothShading; }

private:
	MaterialType materialType_;
	Color albedo_;
	bool smoothShading_;
};

CRT_END
