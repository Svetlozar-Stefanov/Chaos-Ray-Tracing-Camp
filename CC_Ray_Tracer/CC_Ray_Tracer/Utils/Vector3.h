#pragma once
#include <stdlib.h>

class Vector3
{
public:
	Vector3()
	{
		v[0] = 0;
		v[1] = 0;
		v[2] = 0;
	}

	Vector3(float x, float y, float z)
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

	bool operator==(const Vector3& other)
	{
		return x() == other.x() && y() == other.y() && z() == other.z();
	}

	Vector3& operator*=(float n)
	{
		v[0] *= n;
		v[1] *= n;
		v[2] *= n;
		return *this;
	}

	Vector3& operator/=(float n)
	{
		v[0] /= n;
		v[1] /= n;
		v[2] /= n;
		return *this;
	}

	Vector3& operator+=(float n)
	{
		v[0] += n;
		v[1] += n;
		v[2] += n;
		return *this;
	}

	Vector3& operator-=(float n)
	{
		v[0] -= n;
		v[1] -= n;
		v[2] -= n;
		return *this;
	}

	Vector3& operator+=(const Vector3& other)
	{
		v[0] += other.v[0];
		v[1] += other.v[1];
		v[2] += other.v[2];
		return *this;
	}

	Vector3& operator-=(const Vector3& other)
	{
		v[0] -= other.v[0];
		v[1] -= other.v[1];
		v[2] -= other.v[2];
		return *this;
	}

	float getLength() const
	{
		return sqrt(x() * x() + y() * y() + z() * z());
	}

	Vector3 getNormalized() const
	{ 
		return Vector3(v[0] / this->getLength(),
			v[1] / this->getLength(),
			v[2] / this->getLength());
	}

private:
	float v[3];
};

Vector3 operator*(Vector3 v, float n)
{
	v *= n;
	return v;
}

Vector3 operator/(Vector3 v, float n)
{
	v /= n;
	return v;
}

Vector3 operator+(Vector3 v, float n)
{
	v += n;
	return v;
}

Vector3 operator-(Vector3 v, float n)
{
	v -= n;
	return v;
}

Vector3 operator+(Vector3 v1, const Vector3& v2)
{
	v1 += v2;
	return v1;
}

Vector3 operator-(Vector3 v1, const Vector3& v2)
{
	v1 -= v2;
	return v1;
}

void normalize(Vector3& v)
{
	v /= v.getLength();
}

Vector3 cross(const Vector3& v1, const Vector3& v2)
{
	return Vector3(
		v1.y() * v2.z() - v1.z() * v2.y(),
		v1.z() * v2.x() - v1.x() * v2.z(),
		v1.x() * v2.y() - v1.y() * v2.x()
	);
}

float dot(const Vector3& v1, const Vector3& v2) {
	return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();
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
