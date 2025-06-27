#pragma once

#include "crt.h"

CRT_BEGIN

class Resolution {
public:	
	Resolution(unsigned short width, unsigned short height)
		: width_(width), height_(height) {}

	float getAspectRatio() const {
		return static_cast<float>(width_) / static_cast<float>(height_);
	}

	unsigned short width() const { return width_; }
	unsigned short height() const { return height_; }

	unsigned int getPixelCount() const { return width_ * height_; }

	void width(unsigned short width) { width_ = width; }
	void height(unsigned short height) { height_ = height; }

private:
	unsigned short width_;
	unsigned short height_;
};

CRT_END
