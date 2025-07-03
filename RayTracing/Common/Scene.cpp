#include "Scene.h"

#include "JsonParser.h"

CRT_BEGIN

void Scene::fromFile(const std::string& filename) {
	JsonParser parser(filename);
	
	parser.parseSettings(settings_);
	parser.parseCamera(camera_);
	parser.parseMeshes(meshes_);

	camera_.resolution(settings_.resolution);
}

std::vector<Triangle> Scene::getTriangles() const {
		std::vector<Triangle> triangles;
		for (const auto& mesh : meshes_) {
			const std::vector<Triangle>& meshTriangles = mesh.getTriangles();
			triangles.insert(triangles.end(), meshTriangles.begin(), meshTriangles.end());
		}
		return triangles;
	}

CRT_END
