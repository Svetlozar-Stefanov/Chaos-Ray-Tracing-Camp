#pragma once
#include <stdlib.h>

class vec3
{
public:
	vec3()
	{
		v[0] = 0;
		v[1] = 0;
		v[2] = 0;
	}

	vec3(float x, float y, float z)
	{
		v[0] = x;
		v[1] = y;
		v[2] = z;
	}

	const float operator[](int i) const
	{
		return v[i];
	}

	float& operator[](int i)
	{
		return v[i];
	}

	const float x() const
	{
		return v[0];
	}

	const float y() const
	{
		return v[1];
	}

	const float z() const
	{
		return v[2];
	}

	bool operator==(const vec3& other)
	{
		return x() == other.x() && y() == other.y() && z() == other.z();
	}

	vec3& operator*=(float n)
	{
		v[0] *= n;
		v[1] *= n;
		v[2] *= n;
		return *this;
	}

private:
	float v[3];
};

vec3 operator*(vec3 v, float n)
{
	v *= n;
	return v;
}

float rand_float() {
	return rand() / (RAND_MAX + 1.0f);
}

float rand_float(float min, float max)
{
	return min + (max - min) * rand_float();
}

int clamp(int min, int n, int max)
{
	if (n < min)
	{
		n = min;
	}
	else if (n > max)
	{
		n = max;
	}
	return n;
}
