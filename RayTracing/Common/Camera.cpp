#include "Camera.h"

#include <iostream>

CRT_BEGIN

Camera::Camera(const Vec3& position, const Vec3& lookAt, const Vec3& up, float focalLength, Resolution resolution)
	: resolution(resolution),
	  position_(position),
	  lookAt_(lookAt),
	  up_(up),
	  focalLength_(focalLength) {
	update();
}

Ray Camera::getRay(float s, float t) const {
	Vec3 direction = (lowerLeftCorner_
		+ s * viewportHorizontal_
		+ t * viewportVertical_) - position_;
	return Ray(position_, direction.normalized()); 
}

void Camera::update() {
	computeBasis();

	float aspectRatio = float(resolution.width()) / float(resolution.height());
	float viewportHeight = 2.0f;
	float viewportWidth = viewportHeight * aspectRatio;

	viewportHorizontal_ = viewportWidth * u();
	viewportVertical_ = -viewportHeight * v();

	lowerLeftCorner_ = position_
		- (focalLength_ * w())
		- (viewportHorizontal_ * 0.5f)
		- (viewportVertical_ * 0.5f);
}

void Camera::computeBasis() {
	Vec3 w = (position_ - lookAt_).normalized();
	Vec3 u = cross(up_, w).normalized();
	Vec3 v = cross(w, u);

	rotationMatrix_ = Mat3(u, v, w);
}

void Camera::updateLookAtFromRotation() {
	float distanceToLookAt = (lookAt_ - position_).length();
	if (distanceToLookAt < math::EPSILON_ZERO) return;
	Vec3 forward = w() * (-1.0f); 
	lookAt_ = position_ + forward * distanceToLookAt;
}

void Camera::dolly(float distance) {
	Vec3 forward = w() * (-1.0f);
	position_ += forward * distance;
	lookAt_ += forward * distance;
	update();
}

void Camera::truck(float distance) {
	Vec3 right = u(); 
	position_ += right * distance;
	lookAt_ += right * distance;
	update();
}

void Camera::pedestal(float distance) {
	Vec3 up = v(); 
	position_ += up * distance;
	lookAt_ += up * distance;
	update();
}

void Camera::pan(float angleRadians) {
	Mat3 rotation = Mat3::rotationAroundAxis(v(), -angleRadians);
	rotationMatrix_ = rotation * rotationMatrix_;

	updateLookAtFromRotation();
	update(); 
}

void Camera::tilt(float angleRadians) {
	Mat3 rotation = Mat3::rotationAroundAxis(u(), angleRadians);
	rotationMatrix_ = rotation * rotationMatrix_;

	updateLookAtFromRotation();
	update();
}

void Camera::roll(float angleRadians) {
	Mat3 rotation = Mat3::rotationAroundAxis(w() * (-1.0f), -angleRadians);
	rotationMatrix_ = rotation * rotationMatrix_;
	update();
}

CRT_END
