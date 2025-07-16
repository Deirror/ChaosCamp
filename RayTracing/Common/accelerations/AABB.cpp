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

CRT_END
