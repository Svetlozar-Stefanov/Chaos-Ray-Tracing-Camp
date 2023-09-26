#pragma once
#include <vector>

#include "Shape.h"
#include "../Math/Vector3.h"
#include "Triangle.h"

using std::vector;

struct BoundingBox
{
public:
	Vector3 max;
	Vector3 min;

	bool intersects(const Ray& ray) const
	{
		bool hit = false;
		for (int side = 0; side < 3; side++)
		{
			double t = (min[side] - ray.getOrigin()[side]) / ray.getDirection()[side];
			if (t <= 0)
			{
				continue;
			}

			Vector3 p = ray.getPoint(t);
			if ((p.x() > min.x() || areEqual(p.x(),min.x(), EPS))
				&& (p.x() < max.x() || areEqual(p.x(), max.x(), EPS))
				&& (p.y() > min.y() || areEqual(p.y(), min.y(), EPS))
				&& (p.y() < max.y() || areEqual(p.y(), max.y(), EPS))
				&& (p.z() > min.z() || areEqual(p.z(), min.z(), EPS))
				&& (p.z() < max.z() || areEqual(p.z(), max.z(), EPS)))
			{
				hit = true;
			}
		}
		for (int side = 0; side < 3; side++)
		{
			double t = (max[side] - ray.getOrigin()[side]) / ray.getDirection()[side];
			if (t <= 0)
			{
				continue;
			}

			Vector3 p = ray.getPoint(t);
			if ((p.x() > min.x() || areEqual(p.x(), min.x(), EPS))
				&& (p.x() < max.x() || areEqual(p.x(), max.x(), EPS))
				&& (p.y() > min.y() || areEqual(p.y(), min.y(), EPS))
				&& (p.y() < max.y() || areEqual(p.y(), max.y(), EPS))
				&& (p.z() > min.z() || areEqual(p.z(), min.z(), EPS))
				&& (p.z() < max.z() || areEqual(p.z(), max.z(), EPS)))
			{
				hit = true;
			}
		}
		return hit;
	}
};

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
		initializeBBox();

		mVertNormals = vector<Vector3>(mVertices.size());
		genVertNormals();
	}

	int getMaterialIndex() const {
		return mMaterialIdx;
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

	void initializeBBox()
	{
		float minX = INT_MAX;
		float minY = INT_MAX;
		float minZ = INT_MAX;

		float maxX = INT_MIN;
		float maxY = INT_MIN;
		float maxZ = INT_MIN;

		for (auto& vert : mVertices)
		{
			if (minX > vert.x())
			{
				minX = vert.x();
			}
			if (minY > vert.y())
			{
				minY = vert.y();
			}
			if (minZ > vert.z())
			{
				minZ = vert.z();
			}

			if (maxX < vert.x())
			{
				maxX = vert.x();
			}
			if (maxY < vert.y())
			{
				maxY = vert.y();
			}
			if (maxZ < vert.z())
			{
				maxZ = vert.z();
			}
		}
		mBBox.max = Vector3(maxX, maxY, maxZ);
		mBBox.min = Vector3(minX, minY, minZ);
	}
};