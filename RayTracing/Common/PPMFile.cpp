#include "PPMFile.h"

CRT_BEGIN

void PPMFile::printColor(const Color& color) {
		m_File << static_cast<int>(color.r) << ' '
				<< static_cast<int>(color.g) << ' '
				<< static_cast<int>(color.b) << '\n';
}

void PPMFile::setHeader() {
	m_File << "P3\n";
	m_File << c_Resolution.getWidth() << " " << c_Resolution.getHeight() << '\n'; 
	m_File << static_cast<int>(c_MaxColorComponent) << '\n';
}


void PPMFile::open(const std::string& filename) {
	m_File.open(filename);
	if (!m_File.is_open()) {
		throw std::runtime_error("Failed to open PPM file: " + filename);
	}
	setHeader();
}

void PPMFile::close() {
	if (m_File.is_open()) {
		m_File.close();
	}
}

CRT_END