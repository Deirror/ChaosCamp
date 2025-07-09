#include "Scene.h"

#include "json/JsonParser.h"

CRT_BEGIN

Scene::Scene(const std::string& filename, const ParseOptions& options) { 
	fromJsonFile(filename, options); 
	buildTriangles();
}

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
	std::vector<SceneTriangle> triangles(totalTrianglesCount());
	unsigned int meshIdx = 0, sceneTriIdx = 0;
	for (const auto& mesh : meshes_) {
		for (unsigned int triIdx = 0; triIdx < mesh.triangleCount(); ++triIdx) {
			triangles[sceneTriIdx] = { sceneTriIdx, meshIdx };
			++sceneTriIdx;
		}
		++meshIdx;
	}
	return triangles;
}

unsigned int Scene::totalTrianglesCount() const {
	unsigned int trianglesCount = 0;
	for (const auto& mesh : meshes_) {
		trianglesCount += mesh.triangleCount();
	}
	return trianglesCount;
}

void Scene::buildTriangles() {
	triangles_.reserve(totalTrianglesCount());
	for (const auto& mesh : meshes_) {
		const auto& meshTriangles = mesh.getTriangles();
		triangles_.insert(triangles_.end(), meshTriangles.begin(), meshTriangles.end());
	}
}

CRT_END
