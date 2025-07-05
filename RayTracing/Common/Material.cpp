#include "Material.h"

CRT_BEGIN

MaterialType fromStringToMaterialType(const std::string& type) {
	if (type == "diffuse") return MaterialType::DIFFUSE;
	if (type == "reflective") return MaterialType::REFLECTIVE;
	if (type == "refractive") return MaterialType::REFRACTIVE;
	CRT_ERROR("Unknown material type");
}

CRT_END
