#pragma once
#include "../Utils/Math/Vector3.h"
#include "../Utils/Math/Color.h"
#include "Ray.h"
#include <cmath>

class LightSource
{
private:
	Vector3 mPosition;
	int mIntensity;
public:
	LightSource(Vector3 position, int intensity)
	{
		mPosition = position;
		mIntensity = intensity;
	}

	const Vector3& getPosition() const
	{
		return mPosition;
	}

	float getIntensity() const
	{
		return mIntensity;
	}
};