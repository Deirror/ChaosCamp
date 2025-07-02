#pragma once

#include "Resolution.h"
#include "Viewport.h"

#include "Mat3.h"
#include "Ray.h"

CRT_BEGIN

class Camera {
public:
	Camera(const Vec3& position, const Vec3& lookAt, const Vec3& up, Resolution resolution, float focalLength = 1.0f);

	Ray getRay(float s, float t) const;

	void position(const Vec3& position) { position_ = position; update(); }
	const Vec3& position() const { return position_; }

	void lookAt(const Vec3& lookAt) { lookAt_ = lookAt; update(); }
	const Vec3& lookAt() const { return lookAt_; }

	void up(const Vec3& up) { up_ = up; update(); }
	const Vec3& up() const { return up_; }

	void focalLength(float focalLength) { focalLength_ = focalLength; update(); }
	float focalLength() const { return focalLength_; }

	float height() { return viewport_.height(); }
	float width() { return viewport_.width(); }

	Vec3 u() const { return rotationMatrix_.row(0); }
	Vec3 v() const { return rotationMatrix_.row(1); }
	Vec3 w() const { return rotationMatrix_.row(2); }

	void move(float dx, float dy, float dz);

	void dolly(float distance);
	void truck(float distance);
	void pedestal(float distance);

	void rotate(float panRadians, float tiltRadians, float rollRadians);

	void pan(float angleRadians);   
	void tilt(float angleRadians); 
	void roll(float angleRadians);

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

	float focalLength_;
	Viewport viewport_;
};

CRT_END
