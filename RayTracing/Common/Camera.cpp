#include "Camera.h"

CRT_BEGIN

Camera::Camera(const Vec3& position, const Vec3& lookAt, const Vec3& up, Resolution resolution, float focalLength)
	: position_(position),
	lookAt_(lookAt), up_(up),
	focalLength_(focalLength),
	resolution_(resolution),
	viewport_(resolution.forViewport(), focalLength) {
	update();
}

Ray Camera::getRay(float s, float t) const {
	Vec3 direction = (viewport_.lowerLeft()
		+ s * viewport_.horizontal()
		+ t * viewport_.vertical()) - position_;
	return Ray(position_, direction.normalized()); 
}

void Camera::update() {
	computeBasis();
	viewport_.update(position_, rotationMatrix_);
}

void Camera::computeBasis() {
	Vec3 w = (position_ - lookAt_).normalized();
	Vec3 u = cross(up_, w).normalized();
	Vec3 v = cross(w, u);

	up_ = v;

	rotationMatrix_.row(0, u);
	rotationMatrix_.row(1, v);
	rotationMatrix_.row(2, w * (-1.f));
}

void Camera::updateLookAtFromRotation() {
	float distanceToLookAt = (lookAt_ - position_).length();
	if (distanceToLookAt < math::EPSILON_ZERO) return;
	Vec3 forward = w() * (-1.0f); 
	lookAt_ = position_ + forward * distanceToLookAt;
}

void Camera::move(float dx, float dy, float dz) {
	Vec3 delta = u() * dx + v() * dy + w() * (-1.0f) * dz;
	position_ += delta;
	lookAt_ += delta;
	update();
}

void Camera::truck(float distance) {
	move(distance, 0.f, 0.f);
}

void Camera::pedestal(float distance) {
	move(0.f, distance, 0.f);
}

void Camera::dolly(float distance) {
	move(0.f, 0.f, distance);
}

void Camera::rotate(float panRadians, float tiltRadians, float rollRadians) {
	if (panRadians != 0.0f) {
		Mat3 yawRotation = Mat3::rotationAroundAxis(v(), -panRadians);
		rotationMatrix_ = yawRotation * rotationMatrix_;
	}

	if (tiltRadians != 0.0f) {
		Mat3 pitchRotation = Mat3::rotationAroundAxis(u(), -tiltRadians);
		rotationMatrix_ = pitchRotation * rotationMatrix_;
	}

	if (rollRadians != 0.0f) {
		Mat3 rollRotation = Mat3::rotationAroundAxis(w() * (-1.0f), -rollRadians);
		rotationMatrix_ = rollRotation * rotationMatrix_;
	}

	updateLookAtFromRotation();
	update();
}

void Camera::pan(float angleRadians) {
	rotate(angleRadians, 0.f, 0.f);
}

void Camera::tilt(float angleRadians) {
	rotate(0.f, angleRadians, 0.f);
}

void Camera::roll(float angleRadians) {
	rotate(0.f, 0.f, angleRadians);
}

void Camera::orbit(float panRadians, float tiltRadians, const Vec3& orbitAxis) {
	Vec3 offset = position_ - lookAt_;
	float radius = offset.length();
	if (radius < math::EPSILON_ZERO) return;

	Vec3 direction = offset / radius;
	Vec3 right = cross(up_, direction).normalized();

	Mat3 pitchRotation = Mat3::rotationAroundAxis(right, tiltRadians);
	offset = pitchRotation * offset;
	up_ = pitchRotation * up_;

	Mat3 yawRotation = Mat3::rotationAroundAxis(orbitAxis, panRadians);
	offset = yawRotation * offset;
	up_ = yawRotation * up_;

	position_ = lookAt_ + offset;
	up_ = up_.normalized();

	update();
}

CRT_END
