#include "PPMFile.h"

CRT_BEGIN

void PPMFile::printColor(const Color& color) {
		file_ << static_cast<int>(color.r) << ' '
				<< static_cast<int>(color.g) << ' '
				<< static_cast<int>(color.b) << '\n';
}

void PPMFile::setHeader() {
	file_ << "P3\n";
	file_ << resolution_.width() << " " << resolution_.height() << '\n'; 
	file_ << static_cast<int>(maxColorComponent_) << '\n';
}


void PPMFile::open(const std::string& filename) {
	file_.open(filename);
	if (!file_.is_open()) {
		throw std::runtime_error("Failed to open PPM file: " + filename);
	}
	setHeader();
}

void PPMFile::close() {
	if (file_.is_open()) {
		file_.close();
	}
}

CRT_END