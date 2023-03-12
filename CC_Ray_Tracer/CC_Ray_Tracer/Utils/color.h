#pragma once
#include "vec3.h"

typedef vec3 color;

color rand_color()
{
	return color(rand_float(), rand_float(), rand_float());
}

color rand_solid_color() {
	color col;
	int n = rand() % 3 + 1;
	for (int i = 0; i < n; i++)
	{
		int idx = rand() % 3;
		col[idx] = 1.0f;
	}
	return col;
}