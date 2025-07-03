#include "PPMFile.h"

#include "error.h"

CRT_BEGIN

void PPMFile::printColor(const Color& color) {
		file_ << static_cast<int>(color.r) << ' '
				<< static_cast<int>(color.g) << ' '
				<< static_cast<int>(color.b) << '\t';
}

void PPMFile::setHeader() {
	file_ << "P3\n";
	file_ << resolution.width() << " " << resolution.height() << '\n'; 
	file_ << static_cast<int>(maxColorComponent) << '\n';
}

void PPMFile::open(const std::string& filename) {
	file_.open(filename);
	CRT_ENSURE(file_.is_open(), "PPM file cannot be opened");
	setHeader();
}

void PPMFile::close() {
	if (file_.is_open()) {
		file_.close();
	}
}

CRT_END;