#include "Mat3.h"

#include "error.h"

CRT_BEGIN

Mat3::Mat3(const Vec3& row0, const Vec3& row1, const Vec3& row2) {
	this->row0(row0);
	this->row1(row1);
	this->row2(row2);
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

Mat3::Mat3() {
	for (unsigned int row = 0; row < 3; ++row) {
		for (unsigned int col = 0; col < 3; ++col) {
			m_[row][col] = 0.f;
		}
	}
}

float& Mat3::operator()(unsigned int row, unsigned int col) {
	CRT_ENSURE(row >= 0 && row < 3 && col >= 0 && col < 3, "Index out of bounds");
	return m_[row][col];
}

float Mat3::operator()(unsigned int row, unsigned int col) const {
	CRT_ENSURE(row >= 0 && row < 3 && col >= 0 && col < 3, "Index out of bounds");
	return m_[row][col];
}

void Mat3::row0(const Vec3& row0) {
	m_[0][0] = row0.x();
	m_[0][1] = row0.y();
	m_[0][2] = row0.z();
}

void Mat3::row1(const Vec3& row1) {
	m_[1][0] = row1.x();
	m_[1][1] = row1.y();
	m_[1][2] = row1.z();
}

void Mat3::row2(const Vec3& row2) {
	m_[2][0] = row2.x();
	m_[2][1] = row2.y();
	m_[2][2] = row2.z();
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

Mat3 Mat3::identity() {
	Mat3 E = Mat3();
	for (unsigned int i  = 0; i < 3; ++i) {
		E(i, i) = 1.f;
	}
	return E;
}

CRT_END
