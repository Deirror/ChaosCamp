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
	rotationMatrix_.row(2, w);
}

void Camera::updateLookAtFromRotation() {
	float distanceToLookAt = (lookAt_ - position_).length();
	if (distanceToLookAt < math::EPSILON_ZERO) return;
	Vec3 forward = w() * (-1.0f); 
	lookAt_ = position_ + forward * distanceToLookAt;
}

void Camera::resolution(Resolution resolution) {
	resolution_ = resolution;
	viewport_ = Viewport(resolution.forViewport());
	viewport_.update(position_, rotationMatrix_);
}

void Camera::fromSceneFile(const Mat3& rotationMatrix, const Vec3& position) {
	position_ = position;
	fromMatrix(rotationMatrix);
}

void Camera::fromMatrix(const Mat3& rotationMatrix) {
	rotationMatrix_ = rotationMatrix;

	lookAt_ = rotationMatrix_.row(2) * (-1.f);
	up_ = rotationMatrix_.row(1);   

	update();
}

void Camera::focalLength(float focalLength) {
	CRT_ENSURE(FLT_IS_POS(focalLength), "Focal length is not positive"); 
	focalLength_ = focalLength; 
	viewport_.focalLength(focalLength);
	viewport_.update(position_, rotationMatrix_); 
}

void Camera::move(float dx, float dy, float dz) {
	Vec3 delta = u() * dx + v() * dy + w() * (-1.0f) * dz;
	position_ += delta;
	lookAt_ += delta;
	update();
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
