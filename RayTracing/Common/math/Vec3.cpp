#include "Vec3.h"

#include "core/error.h"

CRT_BEGIN

float Vec3::operator[](int index) const {
	switch (index) {
	case 0: return x_;
	case 1: return y_;
	case 2: return z_;
	}
	CRT_ERROR("Index out of bounds");
}

float& Vec3::operator[](int index) {
	switch (index) {
	case 0: return x_;
	case 1: return y_;
	case 2: return z_;
	}
	CRT_ERROR("Index out of bounds");
}

Vec3& Vec3::operator+=(const Vec3& other) {
	x_ += other.x_;
	y_ += other.y_;
	z_ += other.z_;
	return *this;
}

Vec3& Vec3::operator-=(const Vec3& other) {
	x_ -= other.x_;
	y_ -= other.y_;
	z_ -= other.z_;
	return *this;
}

Vec3& Vec3::operator*=(float s) {
	x_ *= s;
	y_ *= s;
	z_ *= s;
	return *this;
}

Vec3& Vec3::operator/=(float s) {
	return *this *= 1.0f / s;
}

Vec3 operator+(const Vec3& lhs, const Vec3& rhs) {
	Vec3 result = lhs;
	return result += rhs;
}

Vec3 operator-(const Vec3& lhs, const Vec3& rhs) {
	Vec3 result = lhs;
	return result -= rhs;
}

Vec3 operator*(const Vec3& lhs, const Vec3& rhs) {
	Vec3 result = lhs;
	result.x_ *= rhs.x_;
	result.y_ *= rhs.y_;
	result.z_ *= rhs.z_;
	return result;
}

Vec3 operator*(float s, const Vec3& other) {
	Vec3 result = other;
	return result *= s;
}

Vec3 operator*(const Vec3& other, float s) {
	return s * other;
}

Vec3 operator/(const Vec3& other, float s) {
	return (1.0f / s) * other;
}

Vec3 cross(const Vec3& lhs, const Vec3& rhs) {
	return Vec3(
		lhs.y() * rhs.z() - lhs.z() * rhs.y(),
		lhs.z() * rhs.x() - lhs.x() * rhs.z(),
		lhs.x() * rhs.y() - lhs.y() * rhs.x()
	);
}

Vec3 Vec3::normalized() const {
	return *this / length();
}

void Vec3::normalize() {
	*this /= length();
}

Vec3 Vec3::min(const Vec3& lhs, const Vec3& rhs) {
	return Vec3(
		lhs.x() < rhs.x() ? lhs.x() : rhs.x(),
		lhs.y() < rhs.y() ? lhs.y() : rhs.y(),
		lhs.z() < rhs.z() ? lhs.z() : rhs.z()
	);
}

Vec3 Vec3::max(const Vec3& lhs, const Vec3& rhs) {
	return Vec3(
		lhs.x() > rhs.x() ? lhs.x() : rhs.x(),
		lhs.y() > rhs.y() ? lhs.y() : rhs.y(),
		lhs.z() > rhs.z() ? lhs.z() : rhs.z()
	);
}

Vec3 Vec3::clamp(const Vec3& vec, float min, float max) {
	return Vec3(
		math::clamp(vec.x(), min, max),
		math::clamp(vec.y(), min, max),
		math::clamp(vec.z(), min, max)
	);
}

CRT_END;