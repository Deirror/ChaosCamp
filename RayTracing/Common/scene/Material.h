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
	Material(MaterialType materialType, const std::string& albedoTextureName, bool smoothShading = false, bool backFaceCulling = false, float ior = 1.f)
		: materialType_(materialType), albedoTextureName_(albedoTextureName), smoothShading_(smoothShading), backFaceCulling_(backFaceCulling), ior_(ior) {}

	MaterialType materialType() const { return materialType_; }
	void materialType(MaterialType materialType) { materialType_ = materialType; }

	const std::string& albedoTextureName() const { return albedoTextureName_; }
	void albedoTextureName(const std::string& albedoTextureName) { albedoTextureName_ = albedoTextureName; }
	
	bool smoothShading() const { return smoothShading_; }
	void smoothShading(bool smoothShading) { smoothShading_ = smoothShading; }

	bool backFaceCulling() const { return backFaceCulling_; }
	void backFaceCulling(bool backFaceCulling) { backFaceCulling_ = backFaceCulling; }

	float ior() const { return ior_; }
	void ior(float ior) { ior_ = ior; }

private:
	MaterialType materialType_ = MaterialType::Air;
	std::string albedoTextureName_;

	bool smoothShading_ = false;
	bool backFaceCulling_ = false;

	float ior_ = 1.f;
};

CRT_END
