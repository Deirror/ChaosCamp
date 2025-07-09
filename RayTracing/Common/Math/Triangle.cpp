#include "Triangle.h"

CRT_BEGIN

Vec3 Triangle::normal() const {
	return cross(v1_ - v0_, v2_ - v0_).normalized();
}

float Triangle::area() const {
	return (0.5f * cross(v1_ - v0_, v2_ - v0_).length());
}

float Triangle::area_squared() const {
	return 0.25f * cross(v1_ - v0_, v2_ - v0_).length_squared();
}

bool Triangle::isWindingAntiClockwise() const {
	return (cross(v1_ - v0_, v2_ - v0_).z() > -math::EPSILON_ZERO);
}

bool Triangle::intersect(const Ray& ray, HitRecord& hitRecord, bool cullBackFace) const {
	Vec3 normal = this->normal();

	if (cullBackFace) {
		if (dot(ray.direction(), normal) > -math::EPSILON_ZERO) {
			return false; 
		}
	}

	float rayPlaneDist = dot(normal, v0() - ray.origin());
	float rayProjection = dot(ray.direction(), normal);

	float t = rayPlaneDist / rayProjection;
	if (t < math::EPSILON_RAY) {
		return false;
	}

	Vec3 point = ray.at(t);

	Vec3 e0 = v1() - v0();
	Vec3 v0p = point - v0();
	if (dot(normal, cross(e0, v0p)) < -math::EPSILON_ZERO) {
		return false; 
	}

	Vec3 e1 = v2() - v1();
	Vec3 v1p = point - v1();
	if (dot(normal, cross(e1, v1p)) < -math::EPSILON_ZERO) {
		return false; 
	}

	Vec3 e2 = v2() - v0();
	Vec3 v2p = point - v2();
	if (dot(normal, cross(e2 * (-1.f), v2p)) < -math::EPSILON_ZERO) {
		return false; 
	}

	float areaABC = cross(e0, e2).length();
	if (areaABC < math::EPSILON_ZERO) { 
		return false; 
	}

	float areaPAB = cross(e0, v0p).length();
	float areaPCA = cross(v0p, e2).length();

	float u = areaPCA / areaABC;
	float v = areaPAB / areaABC;
	float w = 1.f - u - v;

	hitRecord.t = t;
	hitRecord.point = point;
	hitRecord.faceNormal = normal;
	hitRecord.hitNormal = (n0() * w + n1() * u + n2() * v).normalized();
	hitRecord.barycentricCoords = Vec3(u, v, w);

	return true;
}

CRT_END
