#pragma once
#include "shape2d.h"
#include "../color.h"

class circle : public shape2d
{
public:
	circle(float radius, point2d origin, color col)
		: shape2d(origin, col)
	{
		r = radius;
	}

	bool contains(point2d pnt) const override
	{
		return (pnt.x - o.x) * (pnt.x - o.x) + (pnt.y - o.y) * (pnt.y - o.y) <= r*r;
	}

private:
	float r;
};