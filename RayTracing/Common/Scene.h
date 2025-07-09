#pragma once

#include <vector>

#include "Color.h"
#include "Mesh.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"

#include "json/ParseOptions.h"

CRT_BEGIN

struct Settings {
	Color backgroundColor;
	Resolution resolution;
};

struct SceneTriangle {
	unsigned int triangleIndex;
	unsigned int meshIndex;
};

class Scene {
public:
	Scene() = default;
	Scene(const std::string& filename, const ParseOptions& options = {});

	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;

	void fromJsonFile(const std::string& filename, const ParseOptions& options = {});

	std::vector<SceneTriangle> triangles() const;

	const std::vector<Mesh>& meshes() const { return meshes_; }
	const std::vector<Light>& lights() const { return lights_; }
	const std::vector<Material>& materials() const { return materials_; }

	const Settings& settings() const { return settings_; }
	const Camera& camera() const { return camera_; }

	const Triangle& triangle(unsigned int index) const { return triangles_[index]; }
	unsigned int totalTrianglesCount() const;

private:
	void buildTriangles();

private:
	std::vector<Mesh> meshes_;
	std::vector<Light> lights_;
	std::vector<Material> materials_;

	std::vector<Triangle> triangles_;

	Camera camera_;
	Settings settings_;
};

CRT_END
