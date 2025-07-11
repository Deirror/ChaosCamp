#pragma once

#include "AspectHeight.h"

CRT_BEGIN

class Resolution {
public:	
	Resolution() = default;
	Resolution(unsigned short width, unsigned short height)
		: width_(width), height_(height) {}

	float aspectRatio() const { return static_cast<float>(width_) / static_cast<float>(height_); }

	unsigned short width() const { return width_; }
	unsigned short height() const { return height_; }

	void width(unsigned short width) { width_ = width; }
	void height(unsigned short height) { height_ = height; }

	AspectHeight forViewport(float viewportHeight = 2.0f) const { return { aspectRatio(), viewportHeight }; }

public:
	static constexpr unsigned short DEFAULT_WIDTH = 1920;
	static constexpr unsigned short DEFAULT_HEIGHT= 1080;

private:
	unsigned short width_ = DEFAULT_HEIGHT;
	unsigned short height_ = DEFAULT_HEIGHT;
};

CRT_END
