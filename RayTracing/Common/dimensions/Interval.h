#pragma once

struct Point {
	unsigned short x, y;
};

struct Interval {
	Interval() = default;
	Interval(unsigned short startX, unsigned short startY, unsigned short endX, unsigned short endY)
		: start(startX, startY), end(endX, endY) {}

	Interval(Point start, Point end)
		: start(start), end(end) {}

	Point start, end;
};
