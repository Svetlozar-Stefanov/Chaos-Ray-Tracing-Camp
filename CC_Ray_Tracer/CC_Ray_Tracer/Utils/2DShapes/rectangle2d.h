#pragma once
#include <cmath>

#include "shape2d.h"

using std::max;
using std::min;

class rectangle2d : public shape2d
{
public:
	rectangle(point2d a, point2d b, point2d origin, color col)
		: shape2d(origin, col)
	{
		s[0] = a + origin;
		s[1] = b + origin;
	}

	bool contains(point2d p) const override
	{
		return min(s[0].x, s[1].x) <= p.x && p.x <= max(s[0].x, s[1].x)
			&& min(s[0].y, s[1].y) <= p.y && p.y <= max(s[0].y, s[1].y);
	}

private:
	point2d s[2];
};