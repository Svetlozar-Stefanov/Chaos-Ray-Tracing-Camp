 #pragma once
#include <cmath>

#include "Shape2d.h"
#include "../Color.h"

using std::max;
using std::min;

class Rectangle2d : public Shape2d
{
public:
	Rectangle2d(Point2d a, Point2d b, Point2d origin, Color col)
		: Shape2d(origin, col)
	{
		s[0] = a + origin;
		s[1] = b + origin;
	}

	bool contains(Point2d p) const override
	{
		return min(s[0].x, s[1].x) <= p.x && p.x <= max(s[0].x, s[1].x)
			&& min(s[0].y, s[1].y) <= p.y && p.y <= max(s[0].y, s[1].y);
	}

private:
	Point2d s[2];
};