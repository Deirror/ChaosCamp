#pragma once

#include "Resolution.h"
#include "Mat3.h"
#include "Ray.h"

CRT_BEGIN

class Camera {
public:
	Camera(const Vec3& position, const Vec3& lookAt, const Vec3& up, float focalLength, Resolution resolution);

	Ray getRay(float s, float t) const;

	void position(const Vec3& position) { position_ = position; update(); }
	const Vec3& position() const { return position_; }

	void lookAt(const Vec3& lookAt) { lookAt_ = lookAt; update(); }
	const Vec3& lookAt() const { return lookAt_; }

	void up(const Vec3& up) { up_ = up; update(); }
	const Vec3& up() const { return up_; }

	void focalLength(float focalLength) { focalLength_ = focalLength; update(); }
	float focalLength() const { return focalLength_; }

	Vec3 u() const { return rotationMatrix_.getRow(0); }
	Vec3 v() const { return rotationMatrix_.getRow(1); }
	Vec3 w() const { return rotationMatrix_.getRow(2); }

	void dolly(float distance);
	void truck(float distance);
	void pedestal(float distance);

	void pan(float angleRadians);   
	void tilt(float angleRadians); 
	void roll(float angleRadians);

public:
	const Resolution resolution;

private:
	void update();
	void computeBasis();

	void updateLookAtFromRotation();
		
private:
	Vec3 position_;
	Vec3 lookAt_;
	Vec3 up_;

	float focalLength_;

	Mat3 rotationMatrix_;

	Vec3 viewportHorizontal_; 
	Vec3 viewportVertical_;  
	Vec3 lowerLeftCorner_;  
};

CRT_END
