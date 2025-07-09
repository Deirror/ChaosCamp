#include "JsonParser.h"

#include <fstream>

#include "error.h"
#include "Scene.h"

#include "JsonKey.h"

CRT_BEGIN

void from_json(const nlohmann::json& j, Vec3& vec) {
	if (!j.is_array() || j.size() != 3) {
		CRT_ERROR("Invalid Vec3 format");
	}

	vec.x(j[0].get<float>());
	vec.y(j[1].get<float>());
	vec.z(j[2].get<float>());
}

void from_json(const nlohmann::json& j, Color& color) {
	if (!j.is_array() || j.size() != 3) {
		CRT_ERROR("Invalid Color format");
	}

	color.r = static_cast<unsigned char>(j[0].get<float>() * 255.f);
	color.g = static_cast<unsigned char>(j[1].get<float>() * 255.f);
	color.b = static_cast<unsigned char>(j[2].get<float>() * 255.f);
}

void from_json(const nlohmann::json& j, Resolution& res) {
	if (!j.is_object() || j.size() != 2) {
		CRT_ERROR("Invalid Resolution format");
	}

	res.width(j[JsonKey::Settings::Image::WIDTH].get<unsigned short>());
	res.height(j[JsonKey::Settings::Image::HEIGHT].get<unsigned short>());
}

void from_json(const nlohmann::json& j, TriangleIndices& indices) {
	if (!j.is_array() || j.size() != 3) {
		CRT_ERROR("Invalid TriangleIndices format");
	}

	indices.v0 = j[0].get<unsigned int>();
	indices.v1 = j[1].get<unsigned int>();
	indices.v2 = j[2].get<unsigned int>();
}

void from_json(const nlohmann::json& j, Mesh& mesh) {
	if (!j.is_object()) {
		CRT_ERROR("Invalid Mesh JSON");
	}

	if (!j.contains(JsonKey::Objects::VERTICES)) { 
		CRT_ERROR("Mesh vertices not found in JSON data");
	}

	if (!j.contains(JsonKey::Objects::TRIANGLES)) {
		CRT_ERROR("Mesh triangles not found in JSON data");
	}

	if (!j.contains(JsonKey::Objects::MATERIAL_INDEX)) {
		CRT_ERROR("Mesh material index not found in JSON data");
	}

	const auto& verticesJson = j[JsonKey::Objects::VERTICES];
	const auto& trianglesIndicesJson = j[JsonKey::Objects::TRIANGLES];

	for (int i = 0; i < verticesJson.size(); i += 3) {
		mesh.emplaceVertex(
			verticesJson[i].get<float>(),
			verticesJson[i + 1].get<float>(),
			verticesJson[i + 2].get<float>()
		);
	}

	for (int i = 0; i < trianglesIndicesJson.size(); i += 3) {
		mesh.emplaceTriangleIndices(
			trianglesIndicesJson[i].get<unsigned int>(),
			trianglesIndicesJson[i + 1].get<unsigned int>(),
			trianglesIndicesJson[i + 2].get<unsigned int>()
		);
	}

	mesh.materialIndex(j[JsonKey::Objects::MATERIAL_INDEX].get<unsigned int>());
}

void from_json(const nlohmann::json& j, Light& light) {
	if (!j.is_object()) {
		CRT_ERROR("Invalid Light JSON");
	}

	if (!j.contains(JsonKey::Lights::INTENSITY)) {
		CRT_ERROR("Light intensity not found in JSON data");
	}

	light.intensity(j[JsonKey::Lights::INTENSITY].get<float>());

	if (!j.contains(JsonKey::Lights::POSITION)) {
		CRT_ERROR("Light position not found in JSON data");
	}

	light.position(j[JsonKey::Lights::POSITION].get<Vec3>());
}

void from_json(const nlohmann::json& j, Material& material) {
	if (!j.is_object()) {
		CRT_ERROR("Invalid Material JSON");
	}

	if (!j.contains(JsonKey::Material::TYPE)) {
		CRT_ERROR("Material type not found in JSON data");
	}

	material.materialType(crt::fromStringToMaterialType(j[JsonKey::Material::TYPE].get<std::string>()));

	if (!j.contains(JsonKey::Material::ALBEDO)) {
		CRT_ERROR("Material albedo not found in JSON data");
	}

	Color color = j[JsonKey::Material::ALBEDO].get<Color>();
	material.albedo(color);

	if (!j.contains(JsonKey::Material::SMOOTH_SHADING)) {
		CRT_ERROR("Material smooth shading not found in JSON data");
	}

	material.smoothShading(j[JsonKey::Material::SMOOTH_SHADING].get<bool>());
}

void from_json(const nlohmann::json& j, Mat3& matrix) {
	if (!j.is_array() || j.size() != 9) {
		CRT_ERROR("Invalid Mat3 format");
	}

	Vec3 row0(j[0].get<float>(), j[1].get<float>(), j[2].get<float>());
	Vec3 row1(j[3].get<float>(), j[4].get<float>(), j[5].get<float>());
	Vec3 row2(j[6].get<float>(), j[7].get<float>(), j[8].get<float>());

	matrix.row0(row0);
	matrix.row1(row1);
	matrix.row2(row2);
}

void JsonParser::parseFile(const std::string& filename) {
	std::ifstream in(filename);
	CRT_ENSURE(in.is_open(), "Json file is not opened");
	in >> jsonData_;
}

void JsonParser::parseSettings(Settings& settings) {
	if (!jsonData_.contains(JsonKey::SETTINGS)) {
		CRT_ERROR("Settings not found in JSON data");
	}

	const auto& settingsJson = jsonData_[JsonKey::SETTINGS];
	if (!settingsJson.contains(JsonKey::Settings::BACKGROUND_COLOR)) {
		CRT_ERROR("Background color not found in JSON data");
	}

	settings.backgroundColor = settingsJson[JsonKey::Settings::BACKGROUND_COLOR].get<Color>();

	if (!settingsJson.contains(JsonKey::Settings::IMAGE_SETTINGS)) {
		CRT_ERROR("Image settings not found in JSON data");
	}

	settings.resolution = settingsJson[JsonKey::Settings::IMAGE_SETTINGS].get<Resolution>();
}

void JsonParser::parseCamera(Camera& camera) {
	if (!jsonData_.contains(JsonKey::CAMERA)) {
		CRT_ERROR("Camera not found in JSON data");
	}

	const auto& cameraJson = jsonData_[JsonKey::CAMERA];
	if (!cameraJson.contains(JsonKey::Camera::MATRIX)) {
		CRT_ERROR("Camera matrix not found in JSON data");
	}

	Mat3 matrix = cameraJson[JsonKey::Camera::MATRIX].get<Mat3>();

	if (!cameraJson.contains(JsonKey::Camera::POSITION)) {
		CRT_ERROR("Camera position not found in JSON data");
	}

	camera.fromSceneFile(matrix, cameraJson[JsonKey::Camera::POSITION].get<Vec3>());
}

void JsonParser::parseLights(std::vector<Light>& lights) {
	if (!jsonData_.contains(JsonKey::LIGHTS)) {
		CRT_ERROR("Lights not found in JSON data");
	}

	const auto& lightsJson = jsonData_[JsonKey::LIGHTS];
	for (const auto& lightJson : lightsJson) {
		Light light = lightJson.get<Light>();
		lights.emplace_back(std::move(light));
	}
}

void JsonParser::parseMeshes(std::vector<Mesh>& meshes) {
	if (!jsonData_.contains(JsonKey::OBJECTS)) {
		CRT_ERROR("Objects not found in JSON data");
	}

	const auto& objectsJson = jsonData_[JsonKey::OBJECTS];
	for (const auto& meshJson : objectsJson) {
		Mesh mesh = meshJson.get<Mesh>();
		mesh.computeVertexNormals();
		meshes.emplace_back(std::move(mesh));
	}
}

void JsonParser::parseMaterials(std::vector<Material>& materials) {
	if (!jsonData_.contains(JsonKey::MATERIALS)) {
		CRT_ERROR("Materials not found in JSON data");
	}

	const auto& materialsJson = jsonData_[JsonKey::MATERIALS];
	for (const auto& materialJson : materialsJson) {
		Material material = materialJson.get<Material>();
		materials.emplace_back(std::move(material));
	}
}

CRT_END

