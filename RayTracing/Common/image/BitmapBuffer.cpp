#include "BitmapBuffer.h"

#include "third-party/stb_image.h"

CRT_BEGIN

BitmapBuffer::BitmapBuffer(const std::string& filepath) {
	stbi_set_flip_vertically_on_load(1);

	int width = 0, height = 0, bpp = 0;
	unsigned char* rawData = stbi_load(filepath.c_str(), &width, &height, &bpp, kDefaultBpp);

	CRT_ENSURE(rawData != nullptr, "Bitmap data is null");

	resolution_ = Resolution(width, height);
	bpp_ = bpp;

	size_t dataSize = static_cast<size_t>(width) * height * bpp;
	data_.assign(rawData, rawData + dataSize);

	stbi_image_free(rawData);
}

Vec3 BitmapBuffer::at(int x, int y) const {
	CRT_ENSURE(x >= 0 && y >= 0, "Negative coord is passed");
	CRT_ENSURE(x < resolution_.width() && y < resolution_.height(), "Pixel coords out of bounds");

	size_t index = (static_cast<size_t>(y) * resolution_.width() + x) * bpp_;

	CRT_ENSURE(index + bpp_ <= data_.size(), "Pixel data out of range");

	float r = data_[index] / 255.f;
	float g = data_[index + 1] / 255.f;
	float b = data_[index + 2] / 255.f;

	return Vec3(r, g, b);
}

CRT_END

