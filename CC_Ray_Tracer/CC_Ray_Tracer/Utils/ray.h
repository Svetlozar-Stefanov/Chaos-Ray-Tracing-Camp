#pragma once
#include <limits>
#include "vec3.h"
#include "Material.h"

struct Intersection
{
	float t;
	Material material;

	Intersection()
	{
		t = std::numeric_limits<float>::max();
	}

	void update(float newT, Material newMaterial)
	{
		if (t > newT)
		{
			t = newT;
			material = newMaterial;
		}
	}
};

class ray
{
	vec3 ogin;
	vec3 dir;

public:
	ray(vec3 origin, vec3 direction) {
		ogin = origin;
		dir = direction;
	}

	vec3 Origin() const
	{
		return ogin;
	}
	
	vec3 Direction() const {
		return dir;
	}

	vec3 getPoint(float t) const
	{
		return ogin + dir * t;
	}
};