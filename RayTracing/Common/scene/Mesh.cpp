#include "Mesh.h"

#include "core/error.h"

CRT_BEGIN

void Mesh::emplaceTriangleIndices(int v0, int v1, int v2) { 
	CRT_ENSURE(v0 >= 0 && v1 >= 0 && v2 >= 0, "Negative vertex is passed");
	triangleIndices_.emplace_back(v0, v1, v2); 
}

const TriangleIndices& Mesh::triangleIndices(int index) const {
	CRT_ENSURE(index >= 0 && index < triangleIndices_.size(), "Index out of bounds"); 
	return triangleIndices_[index]; 
}

const Vec3& Mesh::uv(int index) const { 
	CRT_ENSURE(index >= 0 && index < uvs_.size(), "Index out of bounds"); 
	return uvs_[index]; 
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

void Mesh::buildAABB() {
	AABB box;
	for (const Vec3& vertex : vertices_) {
		box.expandToInclude(vertex);
	}
	aabb_ = box;
}

CRT_END
