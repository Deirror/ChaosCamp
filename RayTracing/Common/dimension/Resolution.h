#pragma once

#include "AspectHeight.h"

CRT_BEGIN

class Resolution {
public:	
	Resolution() = default;
	Resolution(int width, int height)
		: width_(width), height_(height) { CRT_ENSURE(width >= 0 && height >= 0, "Negative number is passed"); }

	float aspectRatio() const { return static_cast<float>(width_) / static_cast<float>(height_); }

	int width() const { return width_; }
	void width(int width) { CRT_ENSURE(width >= 0, "Width is negative"); width_ = width; }

	int height() const { return height_; }
	void height(int height) { CRT_ENSURE(height >= 0, "Height is negative"); height_ = height; }

	AspectHeight forViewport(float viewportHeight = kDefaultViewportHeight) const { return { aspectRatio(), viewportHeight }; }

public:
	static constexpr int kDefaultWidth = 1920;
	static constexpr int kDefaultHeight= 1080;
	
	static constexpr float kDefaultViewportHeight = 2.f;

private:
	int width_ = kDefaultWidth;
	int height_ = kDefaultHeight;
};

CRT_END
