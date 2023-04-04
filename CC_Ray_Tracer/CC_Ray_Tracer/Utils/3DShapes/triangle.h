#pragma once
#include "shape.h"
#include "../color.h"

class triangle 
	: public shape {

private:
	vec3 vert[3];
	Material t_material;
		
public:
	triangle(vec3 a, vec3 b, vec3 c, Material material)
	{
		vert[0] = a;
		vert[1] = b;
		vert[2] = c;

		t_material = material;
	}

	vec3 normal() const
	{
		vec3 e0 = vert[1] - vert[0];
		vec3 e1 = vert[2] - vert[0];

		vec3 plane_normal = cross(e0, e1);
		normalize(plane_normal);
		return plane_normal;
	}

	float area() const
	{
		vec3 e0 = vert[1] - vert[0];
		vec3 e1 = vert[2] - vert[0];

		vec3 plane_normal = cross(e0, e1);
		return plane_normal.length() / 2;
	}

	virtual bool intersects(const ray& r, Intersection& intersection) const
	{
		vec3 plane_normal = normal();

		bool intersects_plane = dot(plane_normal, r.Direction()) < 0.0f;

		float rp_dist = dot(plane_normal, vert[0]);
		float r_proj = dot(r.Direction(), plane_normal);
		float t = rp_dist / r_proj;
		vec3 p = r.getPoint(t);

		if (dot(plane_normal, cross(vert[1] - vert[0], p - vert[0])) > 0
			&& dot(plane_normal, cross(vert[2] - vert[1], p - vert[1])) > 0
			&& dot(plane_normal, cross(vert[0] - vert[2], p - vert[2])) > 0)
		{
			intersection.update(t, t_material);
			return true;
		}

		return false;
	}
};