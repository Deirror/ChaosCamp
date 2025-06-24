#pragma once

#include <fstream>

#include "Resolution.h"
#include "Color.h"

CRT_BEGIN

class PPMFile {
public:
	PPMFile(const std::string& filepath, const Resolution& resolution, unsigned char maxColorComponent = 255)
		: c_Resolution(resolution), c_MaxColorComponent(maxColorComponent) { open(filepath); }

	~PPMFile() { close(); }

	void printColor(const Color& color);

	const Resolution& getResolution() const { return c_Resolution; }
	unsigned char getMaxColorComponent() const { return c_MaxColorComponent; }

	void addNewLine() { m_File << '\n'; }
	
private:
	void setHeader();

	void open(const std::string& filename);
	void close();

	const Resolution c_Resolution;
	const unsigned char c_MaxColorComponent;

	std::ofstream m_File;
};

CRT_END