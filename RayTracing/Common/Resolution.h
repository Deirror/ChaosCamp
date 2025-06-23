#pragma once

#include "crt.h"

CRT_BEGIN

class Resolution {
public:	
	Resolution(unsigned short width, unsigned short height)
		: m_Width(width), m_Height(height) {}

	float getAspectRatio() const {
		return static_cast<float>(m_Width) / static_cast<float>(m_Height);
	}

	unsigned short getWidth() const { return m_Width; }
	unsigned short getHeight() const { return m_Height; }

	unsigned int getPixelCount() const { return m_Width * m_Height; }

	void setWidth(unsigned short width) { m_Width = width; }
	void setHeight(unsigned short height) { m_Height = height; }

private:
	unsigned short m_Width;
	unsigned short m_Height;
};

CRT_END
