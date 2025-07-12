#pragma once

#include <vector>

#include "math/Triangle.h"
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
		uvs_(uvs) { computeVertexNormals(); }

	void materialIndex(unsigned int materialIndex) { materialIndex_ = materialIndex; }
	unsigned int materialIndex() const { return materialIndex_; }

	void addVertex(const Vec3& vertex) { vertices_.push_back(vertex); }
	void emplaceVertex(float x, float y, float z) { vertices_.emplace_back(x, y, z); }

	void addTriangleIndices(const TriangleIndices& triangleIndices) { triangleIndices_.push_back(triangleIndices); }
	void emplaceTriangleIndices(unsigned int v0, unsigned int v1, unsigned int v2) { triangleIndices_.emplace_back(v0, v1, v2); }

	void addUv(const Vec3& uv) { uvs_.push_back(uv); }
	void emplaceUv(float x, float y, float z) { uvs_.emplace_back(x, y, z); }

	std::vector<Triangle> getTriangles() const;

	const TriangleIndices& triangleIndices(unsigned int idx) const { CRT_ENSURE(idx < triangleIndices_.size(), "Index out of bounds"); return triangleIndices_[idx]; }
	const Vec3& uv(unsigned int idx) const { CRT_ENSURE(idx < uvs_.size(), "Index out of bounds"); return uvs_[idx]; }

	unsigned int triangleCount() const { return static_cast<unsigned int>(triangleIndices_.size()); }

	void computeVertexNormals();

private:
	unsigned int materialIndex_ = 0; 

	std::vector<Vec3> vertices_;
	std::vector<TriangleIndices> triangleIndices_;
	std::vector<Vec3> uvs_;

	std::vector<Vec3> vertexNormals_;
};

CRT_END
