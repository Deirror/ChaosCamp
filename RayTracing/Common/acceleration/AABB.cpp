#include "AABB.h"

CRT_BEGIN

void AABB::expandToInclude(const Vec3& point) {
	min_ = Vec3::min(min_, point);
	max_ = Vec3::max(max_, point);
}

void AABB::expandToInclude(const AABB& box) {
	expandToInclude(box.min_);
	expandToInclude(box.max_);
}

std::pair<AABB, AABB> AABB::split(unsigned int axisIdx) const {
	float middle = (min_[axisIdx] + max_[axisIdx]) * 0.5f;

	AABB leftAABB = *this;
	AABB rightAABB = *this;

	leftAABB.max_[axisIdx] = middle;
	rightAABB.min_[axisIdx] = middle;

	return { leftAABB, rightAABB };
}

CRT_END
