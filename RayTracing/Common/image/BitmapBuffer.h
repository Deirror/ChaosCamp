#pragma once

#include <vector>
#include <string>

#include "dimension/Resolution.h"
#include "math/Vec3.h"

CRT_BEGIN

class BitmapBuffer {
public:
	BitmapBuffer() = default;
	explicit BitmapBuffer(const std::string& filepath);

	const Resolution& resolution() const { return resolution_; }
	const std::vector<unsigned char>& data() const { return data_; }

	int bpp() const { return bpp_; }

	Vec3 at(int x, int y) const; 

public:
	static constexpr int kDefaultBpp = 3; 

private:
	Resolution resolution_;
	std::vector<unsigned char> data_;

	int bpp_ = kDefaultBpp;
};

CRT_END
