#pragma once

#include "Vec3.h"

CRT_BEGIN

class Triangle {
public:
	Triangle(Vec3 v0, Vec3 v1, Vec3 v2);

	const Vec3& v0() const { return v0_; }
	const Vec3& v1() const { return v1_; }
	const Vec3& v2() const { return v2_; }

	void v0(const Vec3& v0) { v0_ = v0; }
	void v1(const Vec3& v1) { v1_ = v1; }
	void v2(const Vec3& v2) { v2_ = v2; }
	
	Vec3 normal() const;

	float area() const;
	float area_squared() const;

private:
	bool isWindingAntiClockwise() const;

private:
	Vec3 v0_, v1_, v2_;
};

CRT_END
