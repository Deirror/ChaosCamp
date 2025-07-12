#include "BitmapBuffer.h"

#include "third-party/stb_image.h"

CRT_BEGIN

BitmapBuffer::BitmapBuffer(const std::string& filepath) {
	stbi_set_flip_vertically_on_load(1);

	int width = 0, height = 0, bpp = 0;
	unsigned char* rawData = stbi_load(filepath.c_str(), &width, &height, &bpp, DEFAULT_BPP);

	CRT_ENSURE(rawData != nullptr, "Bitmap data is null");

	resolution_ = Resolution(static_cast<unsigned short>(width), static_cast<unsigned short>(height));
	bpp_ = bpp;

	size_t dataSize = static_cast<size_t>(width) * height * bpp;
	data_.assign(rawData, rawData + dataSize);

	stbi_image_free(rawData);
}

Vec3 BitmapBuffer::at(unsigned short x, unsigned short y) const {
	CRT_ENSURE(x < resolution_.width() && y < resolution_.height(), "Pixel coordinates out of bounds");

	size_t index = (static_cast<size_t>(y) * resolution_.width() + x) * bpp_;

	CRT_ENSURE(index + bpp_ <= data_.size(), "Pixel data out of range");

	float r = data_[index] / 255.0f;
	float g = data_[index + 1] / 255.0f;
	float b = data_[index + 2] / 255.0f;

	return Vec3(r, g, b);
}

CRT_END

