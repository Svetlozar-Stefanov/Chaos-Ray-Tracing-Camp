#pragma once
#include "Vector3.h"
#include <time.h>

typedef Vector3 Color;

inline Color rand_color()
{
	srand(time(NULL));
	return Color(rand_float(), rand_float(), rand_float());
}

inline Color rand_solid_color() {
	srand(time(NULL));
	Color col;
	int n = rand() % 3 + 1;
	for (int i = 0; i < n; i++)
	{
		int idx = rand() % 3;
		col[idx] = 1.0f;
	}
	return col;
}