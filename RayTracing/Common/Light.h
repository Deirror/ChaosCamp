#pragma once

#include "Math/Vec3.h"

CRT_BEGIN

class Light {
public:
	Light() = default;

	Light(float intensity, const Vec3& position)
		: intensity_(intensity), position_(position) {}

	void intensity(float intensity) { intensity_ = intensity; }
	float intensity() const { return intensity_; }

	void position(const Vec3& position) { position_ = position; }
	Vec3 position() const { return position_; }

private:
	float intensity_ = 0.f;
	Vec3 position_;
};

CRT_END

