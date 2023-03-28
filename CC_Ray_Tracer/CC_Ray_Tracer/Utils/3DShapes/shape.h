#pragma once
#include "../ray.h"

class shape {
public:
	virtual bool intersects(const ray& r) const = 0;
};