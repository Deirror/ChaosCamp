#include "Material.h"

CRT_BEGIN

MaterialType fromStringToMaterialType(const std::string& type) {
	if (type == "diffuse") return MaterialType::Diffuse;
	if (type == "reflective") return MaterialType::Reflective;
	if (type == "refractive") return MaterialType::Refractive;
	if (type == "constant") return MaterialType::Constant;
	return MaterialType::Air;
}

CRT_END
