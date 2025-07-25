#pragma once

#include "core/crt.h"

CRT_BEGIN

struct Point {
	int x, y;
};

struct Interval {
	Interval() = default;
	Interval(int startX, int startY, int endX, int endY)
		: start(startX, startY), end(endX, endY) {}

	Interval(const Point& start, const Point& end)
		: start(start), end(end) {}

	Point start, end;
};

CRT_END
