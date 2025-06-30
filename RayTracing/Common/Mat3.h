#pragma once

#include "Vec3.h"

CRT_BEGIN

class Mat3 {
public:
	Mat3() = default;
	Mat3(const Vec3& row0, const Vec3& row1, const Vec3& row2);

	friend Mat3 operator*(const Mat3& lhs, const Mat3& rhs);
	friend Vec3 operator*(const Mat3& mat, const Vec3& vec);

	float get(int row, int col) const;
	Vec3 getRow(int row) const;

	void set(int row, int col, float value);
	void setRow(int row, const Vec3& vec);

	static Mat3 rotationAroundAxis(const Vec3& axis, float angleRadians);

private:
	float m_[3][3];
};

CRT_END
