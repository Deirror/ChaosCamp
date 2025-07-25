#pragma once

#include <vector>
#include <unordered_map>

#include "acceleration/KDTree.h"
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
	unsigned int bucketSize = 0;
};

struct RaySettings {
	unsigned int maxDepth = 1;
	unsigned int giRays = 0;
	unsigned int spp = 0;
	bool reflection = false;
	bool refraction = false;
};

struct KDTreeSettings {
	unsigned int maxDepth = 0;
	unsigned int trianglesPerBox = 0;
};

struct Settings {
	std::string backgroundAlbedoTextureName;
	ImageSettings imageSettings;
	RaySettings raySettings;
	KDTreeSettings kdTreeSettings;
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
	const Triangle& triangle(int index) const { CRT_ENSURE(index >= 0 && index < triangles_.size(), "Index out of bounds"); return triangles_[index]; }

	const std::vector<SceneTriangle>& sceneTriangles() const { return sceneTriangles_; }
	const SceneTriangle& sceneTriangle( int index) const { CRT_ENSURE(index >= 0 &&index < sceneTriangles_.size(), "Index out of bounds"); return sceneTriangles_[ index]; }

	const std::vector<Mesh>& meshes() const { return meshes_; }
	const Mesh& mesh(int index) const { CRT_ENSURE(index >= 0 && index < meshes_.size(), "Index out of bounds"); return meshes_[index]; }

	const std::vector<Light>& lights() const { return lights_; }
	const Light& light(int index) const { CRT_ENSURE(index >= 0 && index < lights_.size(), "Index out of bounds"); return lights_[ index]; }
	Light& light(int index) { CRT_ENSURE(index >= 0 && index < lights_.size(), "Index out of bounds"); return lights_[ index]; }

	const std::vector<Material>& materials() const { return materials_; }
	const Material& material(int index) const { CRT_ENSURE(index >= 0 && index < materials_.size(), "Index out of bounds"); return materials_[ index]; }

	const std::vector<Texture>& textures() const { return textures_; }
	const Texture& texture(int index) const { CRT_ENSURE(index >= 0 && index < textures_.size(), "Index out of bounds"); return textures_[ index]; }
	Texture& texture(int index) { CRT_ENSURE(index >= 0 && index < textures_.size(), "Index out of bounds"); return textures_[ index]; }

	const KDTree& kdTree() const { return kdTree_; }
	const AABB& sceneAABB() const { return sceneAABB_; }
	
	const Settings& settings() const { return settings_; }

	const Camera& camera() const { return camera_; }
	Camera& camera() { return camera_; }

	unsigned int totalTrianglesCount() const;
	unsigned int totalUvsCount() const;

	unsigned int textureIndex(const std::string& albedoTextureName) const;

	unsigned int meshLocalTriangleIndex(int meshIndex, int triangleIndex) const;
	unsigned int meshGlobalTriangleIndex(int meshIndex, int triangleIndex) const;

private:
	void buildTriangles();
	void buildAlbedoTextures();
	void buildSceneAABB();

private:
	std::vector<Mesh> meshes_;
	std::vector<Light> lights_;
	std::vector<Material> materials_;
	std::vector<Texture> textures_;

	std::vector<Triangle> triangles_;
	std::vector<SceneTriangle> sceneTriangles_;

	std::unordered_map<std::string, unsigned int> albedoTextures_;

	AABB sceneAABB_;
	KDTree kdTree_;

	Camera camera_;
	Settings settings_;
};

CRT_END
