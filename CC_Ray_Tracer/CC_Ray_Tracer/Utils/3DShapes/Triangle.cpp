#include "Triangle.h"

Vector3 calcPlaneNormal(Vector3 v0, Vector3 v1, Vector3 v2)
{
	Vector3 e0 = v1 - v0;
	Vector3 e1 = v2 - v0;

	Vector3 planeNormal = cross(e0, e1);
	normalize(planeNormal);
	return planeNormal;
}

bool Triangle::intersects(const Ray& r, Intersection& intersection) const
{
	Vector3 planeNormal = getNormal();
	float rProj = dot(planeNormal, r.getDirection());

	//Ray is parallel
	if (areEqual(rProj, 0.0f, EPS))
	{
		return false;
	}

	//Camera Ray hits back of triangle
	if (r.getType() == RayType::Camera && rProj > 0)
	{
		return false;
	}

	float rpDist = dot(vert[0] - r.getOrigin(), planeNormal);

	//Camera Ray hits back of triangle
	if (r.getType() == RayType::Camera && rpDist > 0)
	{
		return false;
	}
	
	float t = rpDist / rProj;

	//Triangle is behind current colosest
	if (t > intersection.getT())
	{
		return false;
	}

	//Check if both rProj and rpDist have the same sign
	if (t < 0)
	{
		return false;
	}

	Vector3 p = r.getPoint(t);

	if (dot(planeNormal, cross(vert[1] - vert[0], p - vert[0])) >= 0
		&& dot(planeNormal, cross(vert[2] - vert[1], p - vert[1])) >= 0
		&& dot(planeNormal, cross(vert[0] - vert[2], p - vert[2])) >= 0)
	{

		float u = cross(p - vert[0], vert[2] - vert[0]).getLength() / cross(vert[1] - vert[0], vert[2] - vert[0]).getLength();
		float v = cross(vert[1] - vert[0], p - vert[0]).getLength() / cross(vert[1]-vert[0], vert[2]-vert[0]).getLength();
		float w = 1 - u - v;

		Vector3 intersectionNormal = normals[1] * u + normals[2] * v + normals[0] * w;
		normalize(intersectionNormal);
		intersection.update(t, planeNormal, intersectionNormal);

		return true;
	}

	return false;
}