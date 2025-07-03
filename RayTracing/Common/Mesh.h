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
	Mesh(const std::vector<Vec3>& vertices, const std::vector<TriangleIndices>& triangleIndices)
		: vertices_(vertices), triangleIndices_(triangleIndices) {}

	void addVertices(const Vec3& vertices) { vertices_.push_back(vertices); }
	void addTriangle(const TriangleIndices& indices) { triangleIndices_.push_back(indices); }

	Triangle getTriangle(size_t index) const;
	std::vector<Triangle> getTriangles() const;

private:
	std::vector<Vec3> vertices_;
	std::vector<TriangleIndices> triangleIndices_;
};

CRT_END
