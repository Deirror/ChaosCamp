#pragma once

#include "crt.h"

CRT_BEGIN

struct AspectHeight {
	AspectHeight() = default;
	AspectHeight(float ratio, float height) : ratio(ratio), height(height) {}

	float width() const { return static_cast<float>(height) * static_cast<float>(ratio); }

	float ratio, height;
};

class Resolution {
public:	
	Resolution(unsigned short width, unsigned short height)
		: width_(width), height_(height) {}

	float aspectRatio() const { return static_cast<float>(width_) / static_cast<float>(height_); }

	unsigned short width() const { return width_; }
	unsigned short height() const { return height_; }

	void width(unsigned short width) { width_ = width; }
	void height(unsigned short height) { height_ = height; }

	AspectHeight forViewport(float viewportHeight = 2.0f) const { return { aspectRatio(), viewportHeight }; }

private:
	unsigned short width_;
	unsigned short height_;
};

CRT_END
