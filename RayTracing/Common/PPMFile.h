#pragma once

#include <fstream>

#include "Resolution.h"
#include "Color.h"

CRT_BEGIN

class PPMFile {
public:
	PPMFile(const std::string& filepath, Resolution resolution, unsigned char maxColorComponent = 255)
		: resolution(resolution), maxColorComponent(maxColorComponent) { open(filepath); }

	~PPMFile() { close(); }

	void printColor(const Color& color);

	void addNewLine() { file_ << '\n'; }

public:
	const Resolution resolution;
	const unsigned char maxColorComponent;
	
private:
	void setHeader();

	void open(const std::string& filename);
	void close();

private:
	std::ofstream file_;
};

CRT_END