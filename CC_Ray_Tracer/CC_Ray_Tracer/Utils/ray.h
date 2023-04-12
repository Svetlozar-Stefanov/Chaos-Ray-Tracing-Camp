#pragma once
#include <limits>
#include "Vector3.h"
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

class Ray
{
	Vector3 mOrigin;
	Vector3 mDirection;

public:
	Ray(Vector3 origin, Vector3 direction) {
		mOrigin = origin;
		mDirection = direction;
	}

	Vector3 getOrigin() const
	{
		return mOrigin;
	}
	
	Vector3 getDirection() const {
		return mDirection;
	}

	Vector3 getPoint(float t) const
	{
		return mOrigin + mDirection * t;
	}
};