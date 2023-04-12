#pragma once
#include "../Vector3.h"

struct Point2d
{
	Point2d()
	{
		x = y = 0;
	}

	Point2d(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	Point2d(const Point2d& o)
	{
		x = o.x;
		y = o.y;
	}

	float x;
	float y;
};

Point2d operator+(Point2d c, const Point2d& o)
{
	c.x += o.x;
	c.y += o.y;
	return c;
}

class Shape2d
{
public:
	Shape2d(Point2d origin, Color col)
	{
		o = origin;
		c = col;
	}
	Color c;

	virtual bool contains(Point2d p) const = 0;
	Point2d o;
};