#pragma once

#include "Resolution.h"
#include "Viewport.h"

#include "Math/Mat3.h"
#include "Math/Ray.h"

CRT_BEGIN

class Camera {
public:
	Camera() = default;
	Camera(const Vec3& position, const Vec3& lookAt, const Vec3& up, Resolution resolution, float focalLength = 1.0f);

	Ray getRay(float s, float t) const;

	void position(const Vec3& position) { position_ = position; update(); }
	const Vec3& position() const { return position_; }

	void lookAt(const Vec3& lookAt) { lookAt_ = lookAt; update(); }
	const Vec3& lookAt() const { return lookAt_; }

	void up(const Vec3& up) { up_ = up; update(); }
	const Vec3& up() const { return up_; }

	void fromSceneFile(const Mat3& rotationMatrix, const Vec3& position);
	void fromMatrix(const Mat3& rotationMatrix);

	void focalLength(float focalLength);
	float focalLength() const { return focalLength_; }

	void resolution(Resolution resolution);
	Resolution resolution() const { return resolution_; }

	float height() const { return resolution_.height(); }
	float width() const { return resolution_.width(); }

	const Viewport& viewport() const { return viewport_; }

	Vec3 u() const { return rotationMatrix_.row(0); }
	Vec3 v() const { return rotationMatrix_.row(1); }
	Vec3 w() const { return rotationMatrix_.row(2); }

	void move(float dx, float dy, float dz);

	void truck(float distance) { move(distance, 0.f, 0.f); }
	void pedestal(float distance) { move(0.f, distance, 0.f); }
	void dolly(float distance) { move(0.f, 0.f, distance); }

	void rotate(float panRadians, float tiltRadians, float rollRadians);

	void pan(float angleRadians) { rotate(angleRadians, 0.f, 0.f); }
	void tilt(float angleRadians) { rotate(0.f, angleRadians, 0.f);  }
	void roll(float angleRadians) { rotate(0.f, 0.f, angleRadians); }

	void orbit(float panRadians, float tiltRadians, const Vec3& orbitAxis = Vec3(0.f, 1.f, 0.f));

private:
	void update();
	void computeBasis();

	void updateLookAtFromRotation();
		
private:
	Vec3 position_;
	Vec3 lookAt_;
	Vec3 up_;

	Mat3 rotationMatrix_;

	Resolution resolution_;

	float focalLength_ = 1.f;
	Viewport viewport_;
};

CRT_END
