#include "Hit.h" 

CRT_BEGIN

bool intersectTriangle(const Ray& ray, const Triangle& triangle, HitRecord& hitRecord) {
	Vec3 normal = triangle.normal();

	if (dot(ray.direction(), normal) > -math::EPSILON_ZERO) {
		return false; 
	}

	float rayPlaneDist = dot(normal, triangle.v0() - ray.origin());
	float rayProjection = dot(ray.direction(), normal);

	float t = rayPlaneDist / rayProjection;
	if (t < math::EPSILON_ZERO) {
		return false; 
	}

	Vec3 point = ray.at(t);

	Vec3 e0 = triangle.v1() - triangle.v0();
	Vec3 v0p = point - triangle.v0();
	if (dot(normal, cross(e0, v0p)) < -math::EPSILON_ZERO) {
		return false; 
	}

	Vec3 e1 = triangle.v2() - triangle.v1();
	Vec3 v1p = point - triangle.v1();
	if (dot(normal, cross(e1, v1p)) < -math::EPSILON_ZERO) {
		return false; 
	}

	Vec3 e2 = triangle.v0() - triangle.v2();
	Vec3 v2p = point - triangle.v2();
	if (dot(normal, cross(e2, v2p)) < -math::EPSILON_ZERO) {
		return false; 
	}

	hitRecord.t = t;
	hitRecord.point = point;
	hitRecord.normal = normal;
	hitRecord.triangle = &triangle;

	return true;
}

CRT_END
