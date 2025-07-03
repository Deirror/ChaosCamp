#pragma once

#include "Math/Mat3.h"
#include "AspectHeight.h"

#include "error.h"

CRT_BEGIN

class Viewport {
public:
	Viewport(float aspectRatio = 1.0f, float height = 2.0f, float focalLength = 1.0f)
		: aspectHeight_(aspectRatio, height), 
		focalLength_(focalLength), 
		lowerLeftCorner_(), horizontal_(), vertical_() { CRT_ENSURE(FLT_IS_POS(focalLength), "Focal length is not positive"); }

	Viewport(const AspectHeight& aspectHeight, float focalLength = 1.0f)
		: aspectHeight_(aspectHeight), 
		focalLength_(focalLength), 
		lowerLeftCorner_(), horizontal_(), vertical_() { CRT_ENSURE(FLT_IS_POS(focalLength), "Focal length is not positive"); }

	float aspectRatio() const { return aspectHeight_.ratio; }
	float height() const { return aspectHeight_.height; }
	float focalLength() const { return focalLength_; }

	float width() const { return aspectHeight_.width(); }

	const Vec3& lowerLeft() const { return lowerLeftCorner_; }
	const Vec3& horizontal() const { return horizontal_; }
	const Vec3& vertical() const { return vertical_; }

	void aspectRatio(float aspectRatio) { CRT_ENSURE(FLT_IS_POS(aspectRatio), "Aspect ratio is not positive"); aspectHeight_.ratio = aspectRatio; }
	void height(float height) { CRT_ENSURE(FLT_IS_POS(height), "Height is not positive"); aspectHeight_.height = height; }
	void focalLength(float focalLength) { CRT_ENSURE(FLT_IS_POS(focalLength), "Focal length is not positive"); focalLength_ = focalLength; }

	void update(const Vec3& cameraPosition, const Mat3& rotation);

private:
	AspectHeight aspectHeight_;
	float focalLength_;

	Vec3 lowerLeftCorner_;
	Vec3 horizontal_;
	Vec3 vertical_;
};

CRT_END
