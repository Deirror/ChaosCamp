#pragma once

#include "math/Vec3.h"
#include "math/Ray.h"

#include <float.h>
#include <utility>

CRT_BEGIN

class AABB {
public:
	AABB() = default;
	AABB(const Vec3& min, const Vec3& max) : min_(min), max_(max) {}

	const Vec3& min() const { return min_; }
	const Vec3& max() const { return max_; }

	void expandToInclude(const Vec3& point);
	void expandToInclude(const AABB& box);

	std::pair<AABB, AABB> split(int axis) const;

private:
	Vec3 min_ = Vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	Vec3 max_ = Vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
};

CRT_END

