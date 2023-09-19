#pragma once
#include "Constants.h"
#include <cmath>

inline bool areEqual(float n1, float n2, float eps)
{
	return std::abs(n1 - n2) < eps;
}

inline float degToRad(float deg) {
	return deg * (PI / 180.0f);
}

inline float toRange(float x, float min, float max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}
