#pragma once

#include <fstream>

#include "dimension/Resolution.h"
#include "Color.h"

CRT_BEGIN

class PPMWriter {
public:
	enum class Format {
		P3,
		P6
	};

public:
	PPMWriter(const std::string& filepath, const Resolution& resolution,
		Format format = Format::P3, int maxColorComp = Color::kMaxComp);

	~PPMWriter() { close(); }

	void write(const std::string& data);
	void write(const char* data, size_t size);

	void print(const Color& color);
	void println() { file_ << '\n'; }

	Format format() const { return format_; }

public:
	const Resolution resolution;
	const int maxColorComp = Color::kMaxComp;
	
private:
	void writeHeader(Format format);

	void open(const std::string& filepath, Format format);
	void close();

private:
	std::ofstream file_;

	Format format_ = Format::P3;
};

CRT_END