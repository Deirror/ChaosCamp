#include "PPMFile.h"

#include "error.h"

CRT_BEGIN

void PPMFile::write(const std::string& str) {
	CRT_ENSURE(ppmMode_ == PPMMode::P3, "Wrong PPM mode");
	file_ << str;
}

void PPMFile::write(const char* data, unsigned int size) {
	CRT_ENSURE(ppmMode_ == PPMMode::P6, "Wrong PPM mode");
	file_.write(data, size);
}

void PPMFile::printColor(Color color) {
	file_ << static_cast<int>(color.r) << ' '
			<< static_cast<int>(color.g) << ' '
			<< static_cast<int>(color.b) << ' ';
}

void PPMFile::setHeader(PPMMode ppmMode) {
	switch (ppmMode) {
		case PPMMode::P3:
			file_ << "P3\n";
			break;
		case PPMMode::P6:
			file_ << "P6\n";
			break;
	}
	file_ << resolution.width() << " " << resolution.height() << '\n'; 
	file_ << static_cast<int>(maxColorComponent) << '\n';
}

void PPMFile::open(const std::string& filename, PPMMode ppmMode) {
	switch (ppmMode) {
		case PPMMode::P3:
			file_.open(filename);
			break;
		case PPMMode::P6:
			file_.open(filename, std::ios::binary);
			break;
	}
	CRT_ENSURE(file_.is_open(), "PPM file cannot be opened");
	setHeader(ppmMode);
}

void PPMFile::close() {
	if (file_.is_open()) {
		file_.close();
	}
}

CRT_END;