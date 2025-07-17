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

	Resolution resolution() const { return resolution_; }
	const std::vector<unsigned char>& data() const { return data_; }

	unsigned short bpp() const { return bpp_; }

	Vec3 at(unsigned short x, unsigned short y) const; // for now support for RGB only

public:
	static constexpr char DEFAULT_BPP = 3; 

private:
	Resolution resolution_;
	std::vector<unsigned char> data_;

	unsigned short bpp_ = DEFAULT_BPP;
};

CRT_END
