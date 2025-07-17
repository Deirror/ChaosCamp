#pragma once

#include <vector>

#include "AABB.h"
#include "math/Triangle.h"

CRT_BEGIN

struct KDTreeNode {
	KDTreeNode() = default;
	KDTreeNode(const AABB& aabb, int left, int right, std::vector<unsigned int>&& triangles)
		: aabb(aabb), left(left), right(right), triangleIndices(std::move(triangles)) {}

	static constexpr unsigned short DEFAULT_BOX_TRIANGLE_COUNT = 4;

	AABB aabb;

	int parent = -1;
	int left = -1;
	int right = -1;

	std::vector<unsigned int> triangleIndices;
};

class KDTree {
public:
	KDTree(unsigned short boxTriangleCount = KDTreeNode::DEFAULT_BOX_TRIANGLE_COUNT,
		unsigned short maxDepth = DEFAULT_MAX_DEPTH) 
		: boxTriangleCount_(boxTriangleCount), maxDepth_(maxDepth) {}

	void build(const AABB& topLevelAABB, const std::vector<Triangle>& triangles);

	const std::vector<KDTreeNode>& nodes() const { return nodes_; }

public:
	static constexpr unsigned short DEFAULT_MAX_DEPTH = 15;
	static constexpr unsigned short AXIS_COUNT = 3;

private:
	unsigned int emplaceNode(KDTreeNode&& node);
	unsigned int emplaceNode(const AABB& aabb, int left, int right, std::vector<unsigned int>&& triangleIndices);

	void buildTree(int parent, unsigned short depth, const std::vector<unsigned int>& triangleIndices, const std::vector<AABB>& triangleAABBs);

private:
	std::vector<KDTreeNode> nodes_;
	const std::vector<Triangle>* triangles_ = nullptr;

	unsigned short boxTriangleCount_ = KDTreeNode::DEFAULT_BOX_TRIANGLE_COUNT;
	unsigned short maxDepth_ = DEFAULT_MAX_DEPTH;
};

CRT_END
