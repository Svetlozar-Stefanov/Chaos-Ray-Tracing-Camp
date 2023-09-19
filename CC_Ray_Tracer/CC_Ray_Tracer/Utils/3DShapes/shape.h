#pragma once
#include "../../SceneComponents/Ray.h"

class Shape {
public:
	virtual bool intersects(const Ray& r, Intersection& intersection) const = 0;
};