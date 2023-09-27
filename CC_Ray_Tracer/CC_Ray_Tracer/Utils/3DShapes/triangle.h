#pragma once
#include "Shape.h"
#include "../Math/UtilFunctions.h"
#include "../Math/Constants.h"
#include "../Math/Vector3.h"
#include "BoundingBox.h"

Vector3 calcPlaneNormal(Vector3 v0, Vector3 v1, Vector3 v2);

class Triangle 
	: public Shape {

private:
	Vector3 vert[3];
	Vector3 normals[3];
	Vector3 mPlaneNormal;
	Material* mMaterial;
	BoundingBox bBox;
		
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
		mMaterial = nullptr;
		vector<Vector3> tmp;
		tmp.push_back(vert[0]);
		tmp.push_back(vert[1]);
		tmp.push_back(vert[2]);
		bBox.initializeBBox(tmp);
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
		mMaterial = nullptr;
		vector<Vector3> tmp;
		tmp.push_back(vert[0]);
		tmp.push_back(vert[1]);
		tmp.push_back(vert[2]);
		bBox.initializeBBox(tmp);
	}

	Triangle(Vector3 a, Vector3 b, Vector3 c,
		Vector3 nA, Vector3 nB, Vector3 nC, Material* material)
	{
		vert[0] = a;
		vert[1] = b;
		vert[2] = c;
		mPlaneNormal = calcNormal();
		normals[0] = nA;
		normals[1] = nB;
		normals[2] = nC;
		mMaterial = material;
		vector<Vector3> tmp;
		tmp.push_back(vert[0]);
		tmp.push_back(vert[1]);
		tmp.push_back(vert[2]);
		bBox.initializeBBox(tmp);
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

	void setMaterial(Material* material)
	{
		mMaterial = material;
	}

	Material* getMaterial() const
	{
		return mMaterial;
	}

	const BoundingBox& getBBox() const
	{
		return bBox;
	}

private:
	Vector3 calcNormal() const
	{
		return calcPlaneNormal(vert[0], vert[1], vert[2]);
	}
};