#include "Scene.h"

#include "json/JsonParser.h"

CRT_BEGIN

Scene::Scene(const std::string& filepath, const ParseOptions& options) { 
	fromJsonFile(filepath, options); 
	buildTriangles();
}

void Scene::fromJsonFile(const std::string& filepath, const ParseOptions& options) {
	JsonParser parser(filepath);
	
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

unsigned int Scene::totalTrianglesCount() const {
	unsigned int trianglesCount = 0;
	for (const auto& mesh : meshes_) {
		trianglesCount += mesh.triangleCount();
	}
	return trianglesCount;
}

void Scene::buildTriangles() {
	unsigned int size = totalTrianglesCount();

	triangles_.reserve(size);
	sceneTriangles_.reserve(size);

	unsigned int meshIdx = 0;
	unsigned int sceneTriIdx = 0;

	for (const auto& mesh : meshes_) {
		const auto& meshTriangles = mesh.getTriangles();

		for (unsigned int triIdx = 0; triIdx < meshTriangles.size(); ++triIdx) {
			triangles_.push_back(meshTriangles[triIdx]);
			sceneTriangles_.push_back({ sceneTriIdx, meshIdx });
			++sceneTriIdx;
		}

		++meshIdx;
	}
}

CRT_END
