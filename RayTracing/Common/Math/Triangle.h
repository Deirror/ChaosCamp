#pragma once

#include "Vec3.h"
#include "Ray.h"

CRT_BEGIN

class Triangle {
public:
	Triangle(const Vec3& v0, const Vec3& v1, const Vec3& v2)
		: v0_(v0), v1_(v1), v2_(v2) {}

	Triangle(const Vec3& v0, const Vec3& v1, const Vec3& v2, const Vec3& n0, const Vec3& n1, const Vec3& n2)
		: v0_(v0), v1_(v1), v2_(v2), n0_(n0), n1_(n1), n2_(n2) {}

	const Vec3& v0() const { return v0_; }
	const Vec3& v1() const { return v1_; }
	const Vec3& v2() const { return v2_; }

	void v0(const Vec3& v0) { v0_ = v0; }
	void v1(const Vec3& v1) { v1_ = v1; }
	void v2(const Vec3& v2) { v2_ = v2; }

	const Vec3& n0() const { return n0_; }
	const Vec3& n1() const { return n1_; }
	const Vec3& n2() const { return n2_; }

	void n0(const Vec3& n0) { n0_ = n0; }
	void n1(const Vec3& n1) { n1_ = n1; }
	void n2(const Vec3& n2) { n2_ = n2; }
	
	Vec3 normal() const;

	float area() const;
	float area_squared() const;

	bool isWindingAntiClockwise() const;

private:
	Vec3 v0_, v1_, v2_;
	Vec3 n0_, n1_, n2_; 
};

CRT_END
