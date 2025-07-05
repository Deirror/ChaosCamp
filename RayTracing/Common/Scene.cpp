#include "Scene.h"

#include "json/JsonParser.h"

CRT_BEGIN

void Scene::fromJsonFile(const std::string& filename, const ParseOptions& options) {
	JsonParser parser(filename);
	
	if (options.hasOption(JsonKey::SETTINGS)) {
		parser.parseSettings(settings_);
		camera_.resolution(settings_.resolution);
	}

	if (options.hasOption(JsonKey::CAMERA)) {
		parser.parseCamera(camera_);
	}

	if (options.hasOption(JsonKey::OBJECTS)) {
		parser.parseMeshes(meshes_);
	}

	if (options.hasOption(JsonKey::LIGHTS)) {
		parser.parseLights(lights_);
	}

	if (options.hasOption(JsonKey::MATERIALS)) {
		parser.parseMaterials(materials_);
	}
}

std::vector<SceneTriangle> Scene::triangles() const {
	std::vector<SceneTriangle> triangles;
	unsigned int meshIndex = 0;
	for (const auto& mesh : meshes_) {
		for (const auto& triangle : mesh.getTriangles()) {
			triangles.emplace_back(triangle, meshIndex);
		}
		meshIndex++;
	}
	return triangles;
}

CRT_END
