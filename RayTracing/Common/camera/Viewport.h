#pragma once

#include "math/Mat3.h"
#include "dimensions/AspectHeight.h"

#include "core/error.h"

CRT_BEGIN

class Viewport {
public:
	Viewport() = default;
	explicit Viewport(const AspectHeight& aspectHeight) : aspectHeight_(aspectHeight) {}

	float aspectRatio() const { return aspectHeight_.ratio; }
	float height() const { return aspectHeight_.height; }

	float width() const { return aspectHeight_.width(); }

	const Vec3& lowerLeft() const { return lowerLeftCorner_; }
	const Vec3& horizontal() const { return horizontal_; }
	const Vec3& vertical() const { return vertical_; }

	void aspectRatio(float aspectRatio) { CRT_ENSURE(FLT_IS_POS(aspectRatio), "Aspect ratio is not positive"); aspectHeight_.ratio = aspectRatio; }
	void height(float height) { CRT_ENSURE(FLT_IS_POS(height), "Height is not positive"); aspectHeight_.height = height; }

	void update(const Vec3& cameraPosition, const Mat3& rotation, float focalLength);

private:
	AspectHeight aspectHeight_;

	Vec3 lowerLeftCorner_;
	Vec3 horizontal_;
	Vec3 vertical_;
};

CRT_END
