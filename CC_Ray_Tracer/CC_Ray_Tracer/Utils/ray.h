#pragma once
#include "vec3.h"

class ray
{
	vec3 o;
	vec3 dir;

public:
	ray(vec3 origin, vec3 direction) {
		o = origin;
		dir = direction;
	}

	vec3 Origin() const
	{
		return o;
	}
	
	vec3 Direction() const {
		return dir;
	}
};