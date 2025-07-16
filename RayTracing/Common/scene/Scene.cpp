#include "Scene.h"

#include "json/JsonParser.h"

CRT_BEGIN

Scene::Scene(const std::string& filepath, const ParseOptions& options) { 
	fromJsonFile(filepath, options); 
	buildTriangles();
	buildAlbedoTextures();
}

void Scene::fromJsonFile(const std::string& filepath, const ParseOptions& options) {
	JsonParser parser(filepath);
	
	if (options.hasOption(JsonKey::SETTINGS)) {
		parser.parseSettings(settings_);
		camera_.resolution(settings_.imageSettings.resolution);
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

	if (options.hasOption(JsonKey::TEXTURES)) {
		parser.parseTextures(textures_);
	}
}

unsigned int Scene::totalTrianglesCount() const {
	unsigned int trianglesCount = 0;
	for (const auto& mesh : meshes_) {
		trianglesCount += mesh.trianglesCount();
	}
	return trianglesCount;
}

unsigned int Scene::totalUvsCount() const {
	unsigned int uvsCount = 0;
	for (const auto& mesh : meshes_) {
		uvsCount += mesh.uvsCount();
	}
	return uvsCount;
}

unsigned int Scene::textureIndex(const std::string& albedoTextureName) const {
	CRT_ENSURE(albedoTextures_.count(albedoTextureName) > 0, "Unknown albedo texture name passed");
	return albedoTextures_.at(albedoTextureName);
}

unsigned int Scene::meshLocalTriangleIndex(unsigned int meshIndex, unsigned int triangleIndex) const {
	unsigned int offset = 0;
	for (unsigned int i = 0; i < meshIndex; ++i) {
		offset += meshes_[i].trianglesCount();
	}
	return triangleIndex - offset;
}

unsigned int Scene::meshGlobalTriangleIndex(unsigned int meshIndex, unsigned int triangleIndex) const {
	unsigned int offset = 0;
	for (unsigned int i = 0; i < meshIndex; ++i) {
		offset += meshes_[i].trianglesCount();
	}
	return triangleIndex + offset;
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

void Scene::buildAlbedoTextures() {
	unsigned int idx = 0;
	for (const auto& texture : textures_) {
		albedoTextures_[texture.name()] = idx;
		++idx;
	}
}

CRT_END
