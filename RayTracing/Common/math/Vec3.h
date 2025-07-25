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

	float operator[](int index) const;
	float& operator[](int index);

	Vec3& operator+=(const Vec3& other);
	Vec3& operator-=(const Vec3& other);

	Vec3& operator*=(float s); 
	Vec3& operator/=(float s);

	float length_squared() const { return (x_ * x_) + (y_ * y_) + (z_ * z_); }
	float length() const { return std::sqrt(length_squared()); }

	Vec3 normalized() const;
	void normalize();

	friend Vec3 operator+(const Vec3& lhs, const Vec3& rhs);
	friend Vec3 operator-(const Vec3& lhs, const Vec3& rhs);
	friend Vec3 operator*(const Vec3& lhs, const Vec3& rhs);
	friend Vec3 operator*(float s, const Vec3& other);
	friend Vec3 operator*(const Vec3& other, float s);
	friend Vec3 operator/(const Vec3& other, float s); 

	friend float dot(const Vec3& lhs, const Vec3& rhs) { return (lhs.x_ * rhs.x_) + (lhs.y_ * rhs.y_) + (lhs.z_ * rhs.z_); }
	friend Vec3 cross(const Vec3& lhs, const Vec3& rhs);

	static Vec3 min(const Vec3& lhs, const Vec3& rhs);
	static Vec3 max(const Vec3& lhs, const Vec3& rhs);

	static Vec3 clamp(const Vec3& vec, float min = 0.f, float max = 1.f);

private:
	float x_ = 0.f, y_ = 0.f, z_ = 0.f;
};

CRT_END

