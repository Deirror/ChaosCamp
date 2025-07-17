#include "KDTree.h"

#include "render/Intersection.h"

CRT_BEGIN

void KDTree::build(const AABB& topLevelAABB, const std::vector<Triangle>& triangles) {
	nodes_.clear();

	triangles_ = &triangles;

	std::vector<unsigned int> triangleIndices(triangles.size());
	for (unsigned int i = 0; i < triangles.size(); ++i) {
		triangleIndices[i] = i;
	}

	std::vector<AABB> triangleAABBs(triangles.size());
	for (unsigned int i = 0; i < triangles.size(); ++i) {
		AABB aabb;

		aabb.expandToInclude(triangles[i].v0());
		aabb.expandToInclude(triangles[i].v1());
		aabb.expandToInclude(triangles[i].v2());

		triangleAABBs[i] = aabb;
	}

	emplaceNode(topLevelAABB, -1, -1, {});
	buildTree(0, 0, triangleIndices, triangleAABBs);
}

unsigned int KDTree::emplaceNode(KDTreeNode&& node) {
	nodes_.emplace_back(std::move(node));
	return static_cast<unsigned int>(nodes_.size() - 1);
}

unsigned int KDTree::emplaceNode(const AABB& aabb, int left, int right, std::vector<unsigned int>&& triangleIndices) {
	nodes_.emplace_back(aabb, left, right, std::move(triangleIndices));
	return static_cast<unsigned int>(nodes_.size() - 1);
}

void KDTree::buildTree(int parent, unsigned short depth, const std::vector<unsigned int>& triangleIndices, const std::vector<AABB>& triangleAABBs) {
	if (maxDepth_ < depth || triangleIndices.size() <= boxTriangleCount_) {
		// nodes_[parent].triangleIndices.insert(nodes_[parent].triangleIndices.end(), triangleIndices.begin(), triangleIndices.end());
		nodes_[parent].triangleIndices = triangleIndices;
		return;
	}

	auto [leftAABB, rightAABB] = nodes_[parent].aabb.split(depth % AXIS_COUNT);

	KDTreeNode leftNode, rightNode;
	leftNode.aabb = leftAABB;
	rightNode.aabb = rightAABB;
	
	for (auto triangleIdx : triangleIndices) {
		const Triangle& triangle = triangles_->at(triangleIdx);

		if (intersect(leftAABB, triangleAABBs[triangleIdx])) {
			leftNode.triangleIndices.push_back(triangleIdx);
		}
		if (intersect(rightAABB, triangleAABBs[triangleIdx])) {
			rightNode.triangleIndices.push_back(triangleIdx);
		}
	}

	if (leftNode.triangleIndices.size() > 0) {
		unsigned int leftIdx = emplaceNode(leftNode.aabb, -1, -1, {});
		nodes_[parent].left = leftIdx;
		buildTree(leftIdx, depth + 1, leftNode.triangleIndices, triangleAABBs);
	}

	if (rightNode.triangleIndices.size() > 0) {
		unsigned int rightIdx = emplaceNode(rightNode.aabb, -1, -1, {});
		nodes_[parent].right = rightIdx;
		buildTree(rightIdx, depth + 1, rightNode.triangleIndices, triangleAABBs);
	}
}

CRT_END
