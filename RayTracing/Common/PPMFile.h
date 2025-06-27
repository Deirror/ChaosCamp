#pragma once

#include <fstream>

#include "Resolution.h"
#include "Color.h"

CRT_BEGIN

class PPMFile {
public:
	PPMFile(const std::string& filepath, const Resolution& resolution, unsigned char maxColorComponent = 255)
		: resolution_(resolution), maxColorComponent_(maxColorComponent) { open(filepath); }

	~PPMFile() { close(); }

	void printColor(const Color& color);

	const Resolution& resolution() const { return resolution_; }
	unsigned char maxColorComponent() const { return maxColorComponent_; }

	void addNewLine() { file_ << '\n'; }
	
private:
	void setHeader();

	void open(const std::string& filename);
	void close();

	const Resolution resolution_;
	const unsigned char maxColorComponent_;

	std::ofstream file_;
};

CRT_END