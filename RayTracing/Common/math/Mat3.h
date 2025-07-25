#pragma once

#include "Vec3.h"

CRT_BEGIN

class Mat3 {
public:
	Mat3();
	Mat3(const Vec3& row0, const Vec3& row1, const Vec3& row2);

	friend Mat3 operator*(const Mat3& lhs, const Mat3& rhs);
	friend Vec3 operator*(const Mat3& mat, const Vec3& vec);

	float& operator()(int row, int col);
	float operator()(int row, int col) const;

	Vec3 row0() const { return Vec3(m_[0][0], m_[0][1], m_[0][2]); }
	void row0(const Vec3& row0);

	Vec3 row1() const { return Vec3(m_[1][0], m_[1][1], m_[1][2]); }
	void row1(const Vec3& row1);

	Vec3 row2() const { return Vec3(m_[2][0], m_[2][1], m_[2][2]); }
	void row2(const Vec3& row2);

	static Mat3 rotationAroundAxis(const Vec3& axis, float radians);

	static Mat3 identity();

private:
	float m_[3][3];
};

CRT_END
