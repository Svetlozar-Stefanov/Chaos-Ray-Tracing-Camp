#pragma once
#include "Shape.h"
#include "../Math/UtilFunctions.h"
#include "../Math/Constants.h"
#include "../Math/Vector3.h"

Vector3 calcPlaneNormal(Vector3 v0, Vector3 v1, Vector3 v2);

class Triangle 
	: public Shape {

private:
	Vector3 vert[3];
	Vector3 normals[3];
	Vector3 mPlaneNormal;
		
public:
	Triangle(Vector3 a, Vector3 b, Vector3 c)
	{
		vert[0] = a;
		vert[1] = b;
		vert[2] = c;
		mPlaneNormal = calcNormal();
		normals[0] = mPlaneNormal;
		normals[1] = mPlaneNormal;
		normals[2] = mPlaneNormal;
	}

	Triangle(Vector3 a, Vector3 b, Vector3 c,
			Vector3 nA, Vector3 nB, Vector3 nC)
	{
		vert[0] = a;
		vert[1] = b;
		vert[2] = c;
		mPlaneNormal = calcNormal();
		normals[0] = nA;
		normals[1] = nB;
		normals[2] = nC;
	}

	Vector3 getNormal() const
	{
		return mPlaneNormal;
	}

	float getArea() const
	{
		Vector3 e0 = vert[1] - vert[0];
		Vector3 e1 = vert[2] - vert[0];

		Vector3 planeNormal = cross(e0, e1);
		return planeNormal.getLength() / 2;
	}

	virtual bool intersects(const Ray& r, Intersection& intersection) const override;

private:
	Vector3 calcNormal() const
	{
		return calcPlaneNormal(vert[0], vert[1], vert[2]);
	}
};