#include "Vec3.h"

#include "error.h"

CRT_BEGIN

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

Vec3& Vec3::operator*=(float scalar) {
	x_ *= scalar;
	y_ *= scalar;
	z_ *= scalar;
	return *this;
}

Vec3& Vec3::operator/=(float scalar) {
	return *this *= 1.0f / scalar;
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

Vec3 operator*(float scalar, const Vec3& other) {
	Vec3 result = other;
	return result *= scalar;
}

Vec3 operator*(const Vec3& other, float scalar) {
	return scalar * other;
}

Vec3 operator/(const Vec3& other, float scalar) {
	return (1.0f / scalar) * other;
}

Vec3 cross(const Vec3& lhs, const Vec3& rhs) {
	// after solving the determinant of the matrix - triangle method
	// | i  j  k  |
	// | x1 y1 z1 |
	// | x2 y2 z2 |
	// we get the following formula for the cross product
	return Vec3(
		lhs.y() * rhs.z() - lhs.z() * rhs.y(),
		lhs.z() * rhs.x() - lhs.x() * rhs.z(),
		lhs.x() * rhs.y() - lhs.y() * rhs.x()
	);
}

Vec3 Vec3::normalized() const {
	float len = length();
	return *this / len;
}

CRT_END;