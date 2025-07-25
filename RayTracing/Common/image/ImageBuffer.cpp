#include "ImageBuffer.h"

CRT_BEGIN

void ImageBuffer::save(const std::string& filepath, PPMWriter::Format format) const {
	PPMWriter PPMWriter(filepath, resolution_, format);

	switch (PPMWriter.format()) {
	case PPMWriter::Format::P3:
		dumpASCII(PPMWriter);
		break;
	case PPMWriter::Format::P6:
		dumpBinary(PPMWriter);
		break;
	}
}

void ImageBuffer::dumpASCII(PPMWriter& file) const {
	CRT_ENSURE(file.format() == PPMWriter::Format::P3, "PPM file format must be P3 for ASCII dump");
	std::ostringstream oss;

	for (int y = 0; y < resolution_.height(); ++y) {
		for (int x = 0; x < resolution_.width(); ++x) {
			const Color& color = pixels_[y * resolution_.width() + x];
			oss << color.r << " " << color.g << " "  << color.b << " ";
		}
		oss << "\n";
	}

	file.write(oss.str());
}

void ImageBuffer::dumpBinary(PPMWriter& file) const {
	CRT_ENSURE(file.format() == PPMWriter::Format::P6, "PPM file format must be P6 for binary dump");

	std::vector<unsigned char> buffer;
	buffer.reserve(pixels_.size() * 3);

	for (const Color& color : pixels_) {
		buffer.push_back(color.r);
		buffer.push_back(color.g);
		buffer.push_back(color.b);
	}

	file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
}

void ImageBuffer::set(int x, int y, const Color& color) { 
	CRT_ENSURE(x >= 0 && y >= 0, "Negative coord is passed");

	int index = y * width() + x;
	CRT_ENSURE(index < pixels_.size(), "Pixel is out of range"); 

	pixels_[index] = color; 
}

const Color& ImageBuffer::at(int x, int y) const {
	CRT_ENSURE(x >= 0 && y >= 0, "Negative coord is passed");

	int index = y * width() + x;
	CRT_ENSURE(index < pixels_.size(), "Pixel is out of range"); 

	return pixels_[index]; 
}

CRT_END