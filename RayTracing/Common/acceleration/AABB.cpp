#include "AABB.h"

#include "core/error.h"

CRT_BEGIN

void AABB::expandToInclude(const Vec3& point) {
	min_ = Vec3::min(min_, point);
	max_ = Vec3::max(max_, point);
}

void AABB::expandToInclude(const AABB& box) {
	expandToInclude(box.min_);
	expandToInclude(box.max_);
}

std::pair<AABB, AABB> AABB::split(int axis) const {
	CRT_ENSURE(axis >= 0 && axis <= 2, "Axis out of bounds");
	float middle = (min_[axis] + max_[axis]) * 0.5f;

	AABB leftAABB = *this;
	AABB rightAABB = *this;

	leftAABB.max_[axis] = middle;
	rightAABB.min_[axis] = middle;

	return { leftAABB, rightAABB };
}

CRT_END
