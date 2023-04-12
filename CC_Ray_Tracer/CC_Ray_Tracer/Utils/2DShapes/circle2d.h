#pragma once
#include "Shape2d.h"
#include "../Color.h"

class Circle2d : public Shape2d
{
public:
	Circle2d(float radius, Point2d origin, Color col)
		: Shape2d(origin, col)
	{
		r = radius;
	}

	bool contains(Point2d pnt) const override
	{
		return (pnt.x - o.x) * (pnt.x - o.x) + (pnt.y - o.y) * (pnt.y - o.y) <= r*r;
	}

private:
	float r;
};