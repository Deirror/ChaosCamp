#include "Camera.h"

CRT_BEGIN

const float Camera::kDefaultFov = math::radians(90.f);
const float Camera::kMinFov = math::radians(10.f);
const float Camera::kMaxFov = math::radians(110.f);

Camera::Camera(const Vec3& position, const Vec3& lookAt, 
	const Vec3& up, const Resolution& resolution, 
	float focalLength, float fov)
	: position_(position),
	lookAt_(lookAt), up_(up),
	focalLength_(focalLength),
	fov_(fov),
	resolution_(resolution),
	viewport_(resolution.forViewport()) {
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
	viewport_.update(position_, rotationMatrix_, focalLength_, fov_);
}

void Camera::computeBasis() {
	Vec3 w = (position_ - lookAt_).normalized();
	Vec3 u = cross(up_, w).normalized();
	Vec3 v = cross(w, u);

	up_ = v;

	rotationMatrix_.row0(u);
	rotationMatrix_.row1(v);
	rotationMatrix_.row2(w);
}

void Camera::updateLookAtFromRotation() {
	float distanceToLookAt = (lookAt_ - position_).length();
	if (distanceToLookAt < math::EPSILON_ZERO) return;

	Vec3 forward = w() * (-1.f); 
	lookAt_ = position_ + forward * distanceToLookAt;
}

void Camera::resolution(const Resolution& resolution) {
	resolution_ = resolution;
	viewport_ = Viewport(resolution.forViewport());
	viewport_.update(position_, rotationMatrix_, focalLength_, fov_);
}

void Camera::fromSceneFile(const Mat3& rotationMatrix, const Vec3& position, float fov) {
	position_ = position;
	fov_ = math::clamp(fov, kMinFov, kMaxFov);
	fromMatrix(rotationMatrix);
}

void Camera::fromMatrix(const Mat3& rotationMatrix) {
	rotationMatrix_ = rotationMatrix;

	lookAt_ = rotationMatrix_.row2() * (-1.f);
	up_ = rotationMatrix_.row1();   

	update();
}

void Camera::focalLength(float focalLength) {
	CRT_ENSURE(FLT_IS_POS(focalLength), "Focal length is not positive"); 
	focalLength_ = focalLength; 
	viewport_.update(position_, rotationMatrix_, focalLength, fov_); 
}

void Camera::fov(float fov) {
	fov_ = math::clamp(fov, kMinFov, kMaxFov); 
	viewport_.update(position_, rotationMatrix_, focalLength_, fov); 
}

void Camera::move(float dx, float dy, float dz) {
	Vec3 delta = u() * dx + v() * dy + w() * (-1.f) * dz;
	position_ += delta;
	lookAt_ += delta;
	update();
}

void Camera::rotate(float panRad, float tiltRad, float rollRad) {
	if (panRad != 0.f) {
		Mat3 yaw = Mat3::rotationAroundAxis(v(), -panRad);
		rotationMatrix_ = yaw * rotationMatrix_;
	}

	if (tiltRad != 0.f) {
		Mat3 pitch = Mat3::rotationAroundAxis(u(), -tiltRad);
		rotationMatrix_ = pitch * rotationMatrix_;
	}

	if (rollRad != 0.f) {
		Mat3 roll = Mat3::rotationAroundAxis(w() * (-1.f), -rollRad);
		rotationMatrix_ = roll * rotationMatrix_;
	}

	updateLookAtFromRotation();
	update();
}

void Camera::orbit(float panRad, float tiltRad, const Vec3& orbitAxis) {
	Vec3 offset = position_ - lookAt_;
	float radius = offset.length();
	if (radius < math::EPSILON_ZERO) return;

	Vec3 direction = offset / radius;
	Vec3 right = cross(up_, direction).normalized();

	Mat3 pitch = Mat3::rotationAroundAxis(right, tiltRad);
	offset = pitch * offset;
	up_ = pitch * up_;

	Mat3 yaw = Mat3::rotationAroundAxis(orbitAxis, panRad);
	offset = yaw * offset;
	up_ = yaw * up_;

	position_ = lookAt_ + offset;
	up_ = up_.normalized();

	update();
}

CRT_END
