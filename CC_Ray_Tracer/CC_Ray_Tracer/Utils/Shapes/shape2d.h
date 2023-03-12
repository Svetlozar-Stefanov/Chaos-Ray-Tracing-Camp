#pragma once
#include "../vec3.h"

struct point2d
{
	point2d()
	{
		x = y = 0;
	}

	point2d(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	point2d(const point2d& o)
	{
		x = o.x;
		y = o.y;
	}

	float x;
	float y;
};

point2d operator+(point2d c, const point2d& o)
{
	c.x += o.x;
	c.y += o.y;
	return c;
}

class shape2d
{
public:
	shape2d(point2d origin, color col)
	{
		o = origin;
		c = col;
	}
	color c;

	virtual bool contains(point2d p) const = 0;
	point2d o;
};