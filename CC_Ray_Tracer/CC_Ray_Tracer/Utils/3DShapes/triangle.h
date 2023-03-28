#pragma once
#include "shape.h"
#include "../color.h"

class triangle 
	: public shape {

private:
	vec3 vert[3];
	color col;
		
public:
	triangle(vec3 a, vec3 b, vec3 c, color color)
	{
		vert[0] = a;
		vert[1] = b;
		vert[2] = c;

		col = color;
	}

	virtual bool intersects(const ray& r) const
	{
		vec3 e0 = vert[1] - vert[0];
		vec3 e1 = vert[2] - vert[0];

		vec3 plane_normal = cross(e0, e1);
		normalize(plane_normal);

		bool intersects_plane = dot(plane_normal, r.Direction());
		//TODO: Intersection with the triangle
		return false;
	}
};