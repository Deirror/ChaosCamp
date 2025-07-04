#pragma once

#include <vector>

#include "Math/Triangle.h"

CRT_BEGIN

struct TriangleIndices {
	unsigned int v0, v1, v2;
};

class Mesh {
public:
	Mesh() = default;
	Mesh(unsigned int materialIndex, const std::vector<Vec3>& vertices, const std::vector<TriangleIndices>& triangleIndices)
		: materialIndex_(materialIndex), vertices_(vertices), triangleIndices_(triangleIndices) { computeVertexNormals(); }

	void materialIndex(unsigned int materialIndex) { materialIndex_ = materialIndex; }
	unsigned int materialIndex() const { return materialIndex_; }

	void addVertices(const Vec3& vertices) { vertices_.push_back(vertices); }
	void addTriangle(const TriangleIndices& indices) { triangleIndices_.push_back(indices); }

	Triangle getTriangle(size_t index) const;
	std::vector<Triangle> getTriangles() const;

	void computeVertexNormals();

private:
	unsigned int materialIndex_ = 0; 

	std::vector<Vec3> vertices_;
	std::vector<TriangleIndices> triangleIndices_;

	std::vector<Vec3> vertexNormals_;
};

CRT_END
