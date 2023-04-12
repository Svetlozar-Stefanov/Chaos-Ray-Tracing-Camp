#pragma once
#include "shape2d.h"
#include "../color.h"

float getTriangleArea(Point2d a, Point2d b, Point2d c)
{
	return abs((a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)) / 2);
}

class Triangle2d : public Shape2d
{
public:
	Triangle2d(Point2d a, Point2d b, Point2d c, Point2d origin, Color col)
		: Shape2d(origin, col)
	{
		p[0] = a + origin;
		p[1] = b + origin;
		p[2] = c + origin;
	}

	bool contains(Point2d pnt) const override
	{
		return getTriangleArea(p[0], p[1], p[2]) ==
			(getTriangleArea(pnt, p[0], p[1]) +
			getTriangleArea(pnt, p[1], p[2]) +
			getTriangleArea(pnt, p[0], p[2]));
	}

private:
	Point2d p[3];
};

