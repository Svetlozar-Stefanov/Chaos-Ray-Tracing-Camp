#pragma once
#include "Shape.h"
#include "../Color.h"

class Triangle 
	: public Shape {

private:
	Vector3 vert[3];
	Material mMaterial;
		
public:
	Triangle(Vector3 a, Vector3 b, Vector3 c, Material material)
	{
		vert[0] = a;
		vert[1] = b;
		vert[2] = c;

		mMaterial = material;
	}

	Vector3 getNormal() const
	{
		Vector3 e0 = vert[1] - vert[0];
		Vector3 e1 = vert[2] - vert[0];

		Vector3 planeNormal = cross(e0, e1);
		normalize(planeNormal);
		return planeNormal;
	}

	float getArea() const
	{
		Vector3 e0 = vert[1] - vert[0];
		Vector3 e1 = vert[2] - vert[0];

		Vector3 planeNormal = cross(e0, e1);
		return planeNormal.getLength() / 2;
	}

	virtual bool intersects(const Ray& r, Intersection& intersection) const
	{
		Vector3 planeNormal = getNormal();

		if (dot(planeNormal, r.getDirection()) > 0.0f)
		{
			return false;
		}

		float rpDist = dot(planeNormal, vert[0]);
		float rProj = dot(r.getDirection(), planeNormal);
		float t = rpDist / rProj;
		Vector3 p = r.getPoint(t);

		if (dot(planeNormal, cross(vert[1] - vert[0], p - vert[0])) > 0
			&& dot(planeNormal, cross(vert[2] - vert[1], p - vert[1])) > 0
			&& dot(planeNormal, cross(vert[0] - vert[2], p - vert[2])) > 0)
		{
			intersection.update(t, mMaterial);
			return true;
		}

		return false;
	}
};