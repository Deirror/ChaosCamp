#pragma once

#include <vector>

#include "Color.h"
#include "Mesh.h"
#include "Camera.h"
#include "Light.h"

CRT_BEGIN

struct Settings {
	Color backgroundColor;
	Resolution resolution;
};

class Scene {
public:
	Scene() = default;
	Scene(const std::string& filename) { fromFile(filename); }

	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;

	void fromFile(const std::string& filename);

	std::vector<Triangle> triangles() const;

	const std::vector<Mesh>& meshes() const { return meshes_; }
	const std::vector<Light>& lights() const { return lights_; }

	const Settings& settings() const { return settings_; }
	const Camera& camera() const { return camera_; }

private:
	std::vector<Mesh> meshes_;
	std::vector<Light> lights_;

	Camera camera_;
	Settings settings_;
};

CRT_END
