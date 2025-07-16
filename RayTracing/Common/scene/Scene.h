#pragma once

#include <vector>
#include <unordered_map>

#include "json/ParseOptions.h"
#include "image/Color.h"
#include "camera/Camera.h"

#include "Mesh.h"
#include "Light.h"
#include "Material.h"
#include "Texture.h"

CRT_BEGIN

struct ImageSettings {
	Resolution resolution;
	unsigned short bucketSize = 0;
};

struct Settings {
	Vec3 backgroundColor;
	ImageSettings imageSettings;
};

struct SceneTriangle {
	unsigned int triangleIndex = 0;
	unsigned int meshIndex = 0;
};

class Scene {
public:
	Scene() = default;
	Scene(const std::string& filepath, const ParseOptions& options = {});

	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;

	void fromJsonFile(const std::string& filepath, const ParseOptions& options = {});

	const std::vector<Triangle>& triangles() const { return triangles_; }
	const Triangle& triangle(unsigned int idx) const { CRT_ENSURE(idx < triangles_.size(), "Index out of bounds"); return triangles_[idx]; }

	const std::vector<SceneTriangle>& sceneTriangles() const { return sceneTriangles_; }
	const SceneTriangle& sceneTriangle(unsigned int idx) const { CRT_ENSURE(idx < sceneTriangles_.size(), "Index out of bounds"); return sceneTriangles_[idx]; }

	const std::vector<Mesh>& meshes() const { return meshes_; }
	const Mesh& mesh(unsigned int idx) const { CRT_ENSURE(idx < meshes_.size(), "Index out of bounds"); return meshes_[idx]; }

	const std::vector<Light>& lights() const { return lights_; }
	const Light& light(unsigned int idx) const { CRT_ENSURE(idx < lights_.size(), "Index out of bounds"); return lights_[idx]; }

	const std::vector<Material>& materials() const { return materials_; }
	const Material& material(unsigned int idx) const { CRT_ENSURE(idx < materials_.size(), "Index out of bounds"); return materials_[idx]; }

	const std::vector<Texture>& textures() const { return textures_; }
	const Texture& texture(unsigned int idx) const { CRT_ENSURE(idx < textures_.size(), "Index out of bounds"); return textures_[idx]; }

	const Settings& settings() const { return settings_; }
	const Camera& camera() const { return camera_; }

	unsigned int totalTrianglesCount() const;
	unsigned int totalUvsCount() const;

	unsigned int textureIndex(const std::string& albedoTextureName) const;
	unsigned int meshLocalTriangleIndex(unsigned int meshIndex, unsigned int triangleIndex) const;
	unsigned int meshGlobalTriangleIndex(unsigned int meshIndex, unsigned int triangleIndex) const;

private:
	void buildTriangles();
	void buildAlbedoTextures();

private:
	std::vector<Mesh> meshes_;
	std::vector<Light> lights_;
	std::vector<Material> materials_;
	std::vector<Texture> textures_;

	std::vector<Triangle> triangles_;
	std::vector<SceneTriangle> sceneTriangles_;

	std::unordered_map<std::string, unsigned int> albedoTextures_;

	Camera camera_;
	Settings settings_;
};

CRT_END
