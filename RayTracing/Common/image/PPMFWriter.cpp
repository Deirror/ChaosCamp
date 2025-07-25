#include "PPMWriter.h"

#include "core/error.h"

CRT_BEGIN

PPMWriter::PPMWriter(const std::string& filepath, const Resolution& resolution, Format format, int maxColorComp) 
	: resolution(resolution), maxColorComp(maxColorComp), format_(format) {
	open(filepath, format);
}

void PPMWriter::write(const std::string& data) {
	CRT_ENSURE(format_ == Format::P3, "Wrong PPM format");

	file_ << data;
}

void PPMWriter::write(const char* data, size_t size) {
	CRT_ENSURE(format_ == Format::P6, "Wrong PPM format");
	CRT_ENSURE(data != nullptr, "Data is null pointer");

	file_.write(data, size);
}

void PPMWriter::print(const Color& color) {
	file_ << color.r << ' ' << color.g << ' ' << color.b << ' ';
}

void PPMWriter::writeHeader(Format Format) {
	switch (Format) {
		case Format::P3:
			file_ << "P3\n";
			break;
		case Format::P6:
			file_ << "P6\n";
			break;
	}

	file_ << resolution.width() << " " << resolution.height() << '\n'; 
	file_ << maxColorComp << '\n';
}

void PPMWriter::open(const std::string& filepath, Format format) {
	switch (format) {
		case Format::P3:
			file_.open(filepath);
			break;
		case Format::P6:
			file_.open(filepath, std::ios::binary);
			break;
	}

	CRT_ENSURE(file_.is_open(), "PPM file cannot be opened");

	writeHeader(format);
}

void PPMWriter::close() {
	if (file_.is_open()) {
		file_.close();
	}
}

CRT_END;