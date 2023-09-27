#pragma once
#include "../Math/Vector3.h"
#include "../../SceneComponents/Ray.h"
#include <vector>
#include <cmath>

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

	void initializeBBox(vector<Vector3>& vertices)
	{
		float minX = INT_MAX;
		float minY = INT_MAX;
		float minZ = INT_MAX;

		float maxX = INT_MIN;
		float maxY = INT_MIN;
		float maxZ = INT_MIN;

		for (auto& vert : vertices)
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
		max = Vector3(maxX, maxY, maxZ);
		min = Vector3(minX, minY, minZ);
	}

	BoundingBox merge(const BoundingBox& bBox) const
	{
		BoundingBox newBBox;
		for (int i = 0; i < 3; i++)
		{
			newBBox.min[i] = std::min(min[i], bBox.min[i]);
		}
		for (int i = 0; i < 3; i++)
		{
			newBBox.max[i] = std::max(max[i], bBox.max[i]);
		}
		return newBBox;
	}

	void split(int axis, BoundingBox& bBox1, BoundingBox& bBox2)
	{
		float mid = (max[axis] - min[axis]) / 2;
		float splitCoord = min[axis] + mid;
		bBox1 = *this;
		bBox2 = *this;

		bBox1.max[axis] = splitCoord;
		bBox2.min[axis] = splitCoord;
	}

	bool boxIntersect(const BoundingBox& other)
	{
		for (int i = 0; i < 3; i++)
		{
			if (other.min[i] > max[i]) { return false; }
			if (other.max[i] < min[i]) { return false; }
		}
		return true;
	}
};