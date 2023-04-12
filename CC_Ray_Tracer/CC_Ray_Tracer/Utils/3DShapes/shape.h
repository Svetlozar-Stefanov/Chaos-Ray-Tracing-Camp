#pragma once
#include "../Ray.h"

class Shape {
public:
	virtual bool intersects(const Ray& r, Intersection& intersection) const = 0;
};