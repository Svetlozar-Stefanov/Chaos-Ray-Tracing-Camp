#pragma once
#include "Vector3.h"

typedef Vector3 Color;

Color rand_color()
{
	return Color(rand_float(), rand_float(), rand_float());
}

Color rand_solid_color() {
	Color col;
	int n = rand() % 3 + 1;
	for (int i = 0; i < n; i++)
	{
		int idx = rand() % 3;
		col[idx] = 1.0f;
	}
	return col;
}