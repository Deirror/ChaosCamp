#include "Mesh.h"

CRT_BEGIN

Triangle Mesh::getTriangle(size_t index) const {
	const auto& tri = triangleIndices_.at(index);
	return Triangle(
		vertices_.at(tri.v0), vertices_.at(tri.v1), vertices_.at(tri.v2),
		vertexNormals_.at(tri.v0), vertexNormals_.at(tri.v1), vertexNormals_.at(tri.v2)
	);
}

std::vector<Triangle> Mesh::getTriangles() const {
	std::vector<Triangle> triangles;
	for (const auto& indices : triangleIndices_) {
		triangles.emplace_back(
			vertices_[indices.v0], vertices_[indices.v1], vertices_[indices.v2],
			vertexNormals_[indices.v0], vertexNormals_[indices.v1], vertexNormals_[indices.v2]
		);
	}
	return triangles;
}

void Mesh::computeVertexNormals() {
	vertexNormals_.resize(vertices_.size(), Vec3());

	for (const auto& tri : triangleIndices_) {
		Vec3 v0 = vertices_[tri.v0];
		Vec3 v1 = vertices_[tri.v1];
		Vec3 v2 = vertices_[tri.v2];

		Vec3 faceNormal = (cross(v1 - v0, v2 - v0)).normalized();

		vertexNormals_[tri.v0] += faceNormal;
		vertexNormals_[tri.v1] += faceNormal;
		vertexNormals_[tri.v2] += faceNormal;
	}

	for (auto& n : vertexNormals_) {
		n = n.normalized();
	}
}

CRT_END
