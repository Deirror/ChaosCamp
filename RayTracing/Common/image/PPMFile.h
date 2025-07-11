#pragma once

#include <fstream>

#include "dimensions/Resolution.h"
#include "Color.h"

CRT_BEGIN

enum class PPMMode {
	P3,
	P6	
};

class PPMFile {
public:
	PPMFile(const std::string& filepath, Resolution resolution, PPMMode ppmMode = PPMMode::P3, unsigned char maxColorComponent = 255)
		: resolution(resolution), maxColorComponent(maxColorComponent), ppmMode_(ppmMode) { open(filepath, ppmMode); }

	~PPMFile() { close(); }

	void write(const std::string& str);
	void write(const char* data, unsigned int size);

	void printColor(Color color);

	void addNewLine() { file_ << '\n'; }

	PPMMode ppmMode() const { return ppmMode_; }

public:
	const Resolution resolution;
	const unsigned char maxColorComponent;
	
private:
	void setHeader(PPMMode ppmMode);

	void open(const std::string& filepath, PPMMode ppmMode);
	void close();

private:
	std::ofstream file_;

	PPMMode ppmMode_ = PPMMode::P3;
};

CRT_END