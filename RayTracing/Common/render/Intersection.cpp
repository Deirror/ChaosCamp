#include "Intersection.h"

#include "math/Triangle.h"
#include "math/Ray.h"

#include "HitRecord.h"

CRT_BEGIN

bool intersect(const Triangle& triangle, const Ray& ray, HitRecord& hitRecord, bool cullBackFace) {
	Vec3 faceNormal = triangle.normal();

	if (cullBackFace) {
		if (dot(ray.direction(), faceNormal) > -math::EPSILON_ZERO) {
			return false;
		}
	}

	float rayPlaneDist = dot(faceNormal, triangle.v0() - ray.origin());
	float rayProjection = dot(ray.direction(), faceNormal);

	float t = rayPlaneDist / rayProjection;
	if (t < math::EPSILON_RAY) {
		return false;
	}

	Vec3 point = ray.at(t);

	Vec3 e0 = triangle.v1() - triangle.v0();
	Vec3 v0p = point - triangle.v0();
	if (dot(faceNormal, cross(e0, v0p)) < -math::EPSILON_ZERO) {
		return false;
	}

	Vec3 e1 = triangle.v2() - triangle.v1();
	Vec3 v1p = point - triangle.v1();
	if (dot(faceNormal, cross(e1, v1p)) < -math::EPSILON_ZERO) {
		return false;
	}

	Vec3 e2 = triangle.v2() - triangle.v0();
	Vec3 v2p = point - triangle.v2();
	if (dot(faceNormal, cross(e2 * (-1.f), v2p)) < -math::EPSILON_ZERO) {
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
	hitRecord.faceNormal = faceNormal;
	hitRecord.hitNormal = (triangle.n0() * w + triangle.n1() * u + triangle.n2() * v).normalized();
	hitRecord.barycentricCoords = Vec3(u, v, w);

	return true;
}

CRT_END
