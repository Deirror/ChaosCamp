#include "ImageBuffer.h"

CRT_BEGIN

void ImageBuffer::save(const std::string& filename, PPMMode ppmMode) const {
	PPMFile ppmFile(filename, resolution_, ppmMode);

	switch (ppmFile.ppmMode()) {
		case PPMMode::P3:
			dumpASCII(ppmFile);
			break;
		case PPMMode::P6:
			dumpBinary(ppmFile);
			break;
	}
}

void ImageBuffer::dumpASCII(PPMFile& ppmFile) const {
	CRT_ENSURE(ppmFile.ppmMode() == PPMMode::P3, "PPM file mode must be P3 for ASCII dump");
	std::ostringstream oss;

	for (int y = 0; y < resolution_.height(); ++y) {
		for (int x = 0; x < resolution_.width(); ++x) {
			Color color = pixels_[y * resolution_.width() + x];
			oss << static_cast<int>(color.r) << " " 
				<< static_cast<int>(color.g) << " " 
				<< static_cast<int>(color.b) << " ";
		}
		oss << "\n";
	}

	ppmFile.write(oss.str());
}

void ImageBuffer::dumpBinary(PPMFile& ppmFile) const {
	CRT_ENSURE(ppmFile.ppmMode() == PPMMode::P6, "PPM file mode must be P6 for binary dump");
	std::vector<unsigned char> buffer;
	buffer.reserve(pixels_.size() * 3);

	for (Color color : pixels_) {
		buffer.push_back(color.r);
		buffer.push_back(color.g);
		buffer.push_back(color.b);
	}

	ppmFile.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
}

void ImageBuffer::set(unsigned short x, unsigned short y, Color color) { 
	unsigned int index = y * width() + x;
	CRT_ENSURE(index < pixels_.size(), "Pixel is out of range"); 
	pixels_[index] = color; 
}

Color ImageBuffer::at(unsigned short x, unsigned short y) const {
	unsigned int index = y * width() + x;
	CRT_ENSURE(index < pixels_.size(), "Pixel is out of range"); 
	return pixels_[index]; 
}

CRT_END