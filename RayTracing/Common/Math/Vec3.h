#pragma once

#include <array>
#include <cmath>

#include "utils.h"

CRT_BEGIN

class Vec3 {
public:
	Vec3() = default;
	Vec3(float x, float y, float z) : x_(x), y_(y), z_(z) {}

	float x() const { return x_; }
	void x(float x) { x_ = x; }

	float y() const { return y_; }
	void y(float y) { y_ = y; }

	float z() const { return z_; }
	void z(float z) { z_ = z; }

	Vec3& operator+=(const Vec3& other);
	Vec3& operator-=(const Vec3& other);

	Vec3& operator*=(float scalar); 
	Vec3& operator/=(float scalar);

	float length_squared() const { return (x_ * x_) + (y_ * y_) + (z_ * z_); }
	float length() const { return std::sqrt(length_squared()); }

	Vec3 normalized() const;

	friend Vec3 operator+(const Vec3& lhs, const Vec3& rhs);
	friend Vec3 operator-(const Vec3& lhs, const Vec3& rhs);
	friend Vec3 operator*(const Vec3& lhs, const Vec3& rhs);
	friend Vec3 operator*(float scalar, const Vec3& other);
	friend Vec3 operator*(const Vec3& other, float scalar);
	friend Vec3 operator/(const Vec3& other, float scalar); 

	friend float dot(const Vec3& lhs, const Vec3& rhs) { return (lhs.x_ * rhs.x_) + (lhs.y_ * rhs.y_) + (lhs.z_ * rhs.z_); }
	friend Vec3 cross(const Vec3& lhs, const Vec3& rhs);

private:
	float x_ = 0.f, y_ = 0.f, z_ = 0.f;
};

CRT_END

