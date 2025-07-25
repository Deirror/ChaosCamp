#pragma once

#include <vector>

#include "math/Triangle.h"
#include "acceleration/AABB.h"

#include "core/error.h"

CRT_BEGIN

struct TriangleIndices {
	unsigned int v0, v1, v2;
};

class Mesh {
public:
	Mesh() = default;
	Mesh(unsigned int materialIndex, 
		const std::vector<Vec3>& vertices, 
		const std::vector<TriangleIndices>& triangleIndices,
		const std::vector<Vec3>& uvs)
		: materialIndex_(materialIndex), 
		vertices_(vertices), 
		triangleIndices_(triangleIndices),
		uvs_(uvs) { computeVertexNormals(); buildAABB(); }

	unsigned int materialIndex() const { return materialIndex_; }
	void materialIndex(int materialIndex) { CRT_ENSURE(materialIndex >= 0, "Negative index"); materialIndex_ = materialIndex; }

	void addVertex(const Vec3& vertex) { vertices_.push_back(vertex); }
	void emplaceVertex(float x, float y, float z) { vertices_.emplace_back(x, y, z); }

	void addTriangleIndices(const TriangleIndices& triangleIndices) { triangleIndices_.push_back(triangleIndices); }
	void emplaceTriangleIndices(int v0, int v1, int v2);

	void addUv(const Vec3& uv) { uvs_.push_back(uv); }
	void emplaceUv(float x, float y, float z) { uvs_.emplace_back(x, y, z); }

	std::vector<Triangle> getTriangles() const;

	const TriangleIndices& triangleIndices(int index) const;
	const Vec3& uv(int index) const;

	size_t uvsCount() const { return uvs_.size(); }
	size_t trianglesCount() const { return triangleIndices_.size(); }

	void computeVertexNormals();
	void buildAABB();

	const AABB& aabb() const { return aabb_; }

private:
	unsigned int materialIndex_ = 0; 

	std::vector<Vec3> vertices_;
	std::vector<TriangleIndices> triangleIndices_;
	std::vector<Vec3> uvs_;

	std::vector<Vec3> vertexNormals_;

	AABB aabb_;
};

CRT_END
