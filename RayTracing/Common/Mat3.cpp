#include "Mat3.h"

#include <cassert>

CRT_BEGIN

Mat3::Mat3(const Vec3& row0, const Vec3& row1, const Vec3& row2) {
	row(0, row0);
	row(1, row1);
	row(2, row2);
}

Mat3 operator*(const Mat3& lhs, const Mat3& rhs) {
	Mat3 result;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			result.m_[i][j] = lhs.m_[i][0] * rhs.m_[0][j] +
							  lhs.m_[i][1] * rhs.m_[1][j] +
							  lhs.m_[i][2] * rhs.m_[2][j];
		}
	}
	return result;
}

Vec3 operator*(const Mat3& mat, const Vec3& vec) {
	return Vec3(
		mat.m_[0][0] * vec.x() + mat.m_[0][1] * vec.y() + mat.m_[0][2] * vec.z(),
		mat.m_[1][0] * vec.x() + mat.m_[1][1] * vec.y() + mat.m_[1][2] * vec.z(),
		mat.m_[2][0] * vec.x() + mat.m_[2][1] * vec.y() + mat.m_[2][2] * vec.z()
	);
}

float& Mat3::operator()(int row, int col) {
	assert(row >= 0 && row < 3 && col >= 0 && col < 3, "Index out of bounds in Mat3::operator()");
	return m_[row][col];
}

float Mat3::operator()(int row, int col) const {
	assert(row >= 0 && row < 3 && col >= 0 && col < 3, "Index out of bounds in Mat3::operator()");
	return m_[row][col];
}

Vec3 Mat3::row(int i) const {
	assert(i >= 0 && i < 3, "Index out of bounds in Mat3::row");
	return Vec3(m_[i][0], m_[i][1], m_[i][2]);
}

void Mat3::row(int row, const Vec3& vec) {
	assert(row >= 0 && row < 3, "Index out of bounds in Mat3::setRow");
	m_[row][0] = vec.x();
	m_[row][1] = vec.y();
	m_[row][2] = vec.z();
}

Mat3 Mat3::rotationAroundAxis(const Vec3& axis, float angleRadians) {
	Vec3 n = axis.normalized();

	float cosA = cos(angleRadians);
	float sinA = sin(angleRadians);
	float oneMinusCosA = 1.0f - cosA;

	float x = n.x();
	float y = n.y();
	float z = n.z();

	float r00 = cosA + x * x * oneMinusCosA;
	float r01 = x * y * oneMinusCosA - z * sinA;
	float r02 = x * z * oneMinusCosA + y * sinA;

	float r10 = y * x * oneMinusCosA + z * sinA;
	float r11 = cosA + y * y * oneMinusCosA;
	float r12 = y * z * oneMinusCosA - x * sinA;

	float r20 = z * x * oneMinusCosA - y * sinA;
	float r21 = z * y * oneMinusCosA + x * sinA;
	float r22 = cosA + z * z * oneMinusCosA;

	return Mat3(
		Vec3(r00, r01, r02),
		Vec3(r10, r11, r12),
		Vec3(r20, r21, r22)
	);
}

CRT_END
