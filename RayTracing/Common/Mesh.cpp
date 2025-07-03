#include "Mesh.h"

CRT_BEGIN

Triangle Mesh::getTriangle(size_t index) const {
	const auto& tri = triangleIndices_.at(index);
	return Triangle{ vertices_.at(tri.v0), vertices_.at(tri.v1), vertices_.at(tri.v2) };
}

std::vector<Triangle> Mesh::getTriangles() const {
	std::vector<Triangle> triangles;
	for (const auto& indices : triangleIndices_) {
		triangles.emplace_back(vertices_[indices.v0], vertices_[indices.v1], vertices_[indices.v2]);
	}
	return triangles;
}

CRT_END
