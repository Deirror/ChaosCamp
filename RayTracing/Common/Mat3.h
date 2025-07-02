#pragma once

#include "Vec3.h"

CRT_BEGIN

class Mat3 {
public:
	Mat3() = default;
	Mat3(const Vec3& row0, const Vec3& row1, const Vec3& row2);

	friend Mat3 operator*(const Mat3& lhs, const Mat3& rhs);
	friend Vec3 operator*(const Mat3& mat, const Vec3& vec);

	float& operator()(int row, int col);
	float operator()(int row, int col) const;

	Vec3 row(int i) const;
	void row(int i, const Vec3& v);

	static Mat3 rotationAroundAxis(const Vec3& axis, float angleRadians);

private:
	float m_[3][3];
};

CRT_END
