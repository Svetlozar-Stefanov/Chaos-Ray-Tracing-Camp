#pragma once
#include "shape2d.h"
#include "../color.h"

float triangle_area(point2d a, point2d b, point2d c)
{
	return abs((a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)) / 2);
}

class triangle2d : public shape2d
{
public:
	triangle2d(point2d a, point2d b, point2d c, point2d origin, color col)
		: shape2d(origin, col)
	{
		p[0] = a + origin;
		p[1] = b + origin;
		p[2] = c + origin;
	}

	bool contains(point2d pnt) const override
	{
		return triangle_area(p[0], p[1], p[2]) ==
			(triangle_area(pnt, p[0], p[1]) +
			triangle_area(pnt, p[1], p[2]) +
			triangle_area(pnt, p[0], p[2]));
	}

private:
	point2d p[3];
};

