#pragma once

#include "Math/Mat3.h"
#include "Math/Ray.h"

#include "dimension/Resolution.h"

#include "Viewport.h"

CRT_BEGIN

class Camera {
public:
	Camera() = default;
	Camera(const Vec3& position, const Vec3& lookAt,
		const Vec3& up, const Resolution& resolution, 
		float focalLength = kDefaultFocalLength, float fov = kDefaultFov);

	Ray getRay(float s, float t) const;

	const Vec3& position() const { return position_; }
	void position(const Vec3& position) { position_ = position; update(); }

	const Vec3& lookAt() const { return lookAt_; }
	void lookAt(const Vec3& lookAt) { lookAt_ = lookAt; update(); }

	const Vec3& up() const { return up_; }
	void up(const Vec3& up) { up_ = up; update(); }

	float focalLength() const { return focalLength_; }
	void focalLength(float focalLength);

	float fov() const { return fov_; }
	void fov(float fov);

	const Resolution& resolution() const { return resolution_; }
	void resolution(const Resolution& resolution);

	void fromSceneFile(const Mat3& rotationMatrix, const Vec3& position, float fov);
	void fromMatrix(const Mat3& rotationMatrix);

	int height() const { return resolution_.height(); }
	int width() const { return resolution_.width(); }

	const Viewport& viewport() const { return viewport_; }

	Vec3 u() const { return rotationMatrix_.row0(); }
	Vec3 v() const { return rotationMatrix_.row1(); }
	Vec3 w() const { return rotationMatrix_.row2(); }

	void move(float dx, float dy, float dz);

	void truck(float dx) { move(dx, 0.f, 0.f); }
	void pedestal(float dy) { move(0.f, dy, 0.f); }
	void dolly(float dz) { move(0.f, 0.f, dz); }

	void rotate(float panRad, float tiltRad, float rollRad);

	void pan(float angleRad) { rotate(angleRad, 0.f, 0.f); }
	void tilt(float angleRad) { rotate(0.f, angleRad, 0.f);  }
	void roll(float angleRad) { rotate(0.f, 0.f, angleRad); }

	void orbit(float panRad, float tiltRad, const Vec3& orbitAxis = Vec3(0.f, 1.f, 0.f));

public:
	static constexpr float kDefaultFocalLength = 1.f;
	static const float kDefaultFov;

	static const float kMinFov;
	static const float kMaxFov;

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

	float fov_ = kDefaultFov;
	float focalLength_ = kDefaultFocalLength;
	Viewport viewport_;
};

CRT_END
