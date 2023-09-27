#pragma once
#include <vector>

#include "Shape.h"
#include "../Math/Vector3.h"
#include "Triangle.h"
#include "BoundingBox.h"

using std::vector;

struct TriangleIndices
{
	TriangleIndices(int index0, int index1, int index2)
	{
		i0 = index0;
		i1 = index1;
		i2 = index2;
	}

	int i0;
	int i1;
	int i2;
};

class Mesh : public Shape
{
public:
	Mesh(const vector<Vector3>& vertices, const vector<TriangleIndices>& indices, int materialIdx)
	{
		mVertices = vertices;
		mIndices = indices;
		mMaterialIdx = materialIdx;
		mBBox.initializeBBox(mVertices);

		mVertNormals = vector<Vector3>(mVertices.size());
		genVertNormals();
	}

	int getMaterialIndex() const {
		return mMaterialIdx;
	}

	const BoundingBox& getBBox() const
	{
		return mBBox;
	}

	vector<Triangle> getTriangles() const
	{
		vector<Triangle> triangles;
		for (int i = 0; i < mIndices.size(); ++i)
		{
			const TriangleIndices& vertIndices = mIndices[i];
			Triangle triangle(
				mVertices[vertIndices.i0],
				mVertices[vertIndices.i1],
				mVertices[vertIndices.i2],
				mVertNormals[vertIndices.i0],
				mVertNormals[vertIndices.i1],
				mVertNormals[vertIndices.i2]);

			triangles.push_back(triangle);
		}
		return triangles;
	}

	virtual bool intersects(const Ray& r, Intersection& intersection) const override;

private:
	vector<Vector3> mVertices;
	vector<Vector3> mVertNormals;
	vector<TriangleIndices> mIndices;
	int mMaterialIdx;

	BoundingBox mBBox;

	void genVertNormals()
	{
		for (const TriangleIndices& idc : mIndices)
		{
			Vector3 normal = calcPlaneNormal(mVertices[idc.i0], mVertices[idc.i1], mVertices[idc.i2]);
			mVertNormals[idc.i0] += normal;
			mVertNormals[idc.i1] += normal;
			mVertNormals[idc.i2] += normal;
		}

		for (int i = 0; i < mVertNormals.size(); i++)
		{
			normalize(mVertNormals[i]);
		}
	}
};