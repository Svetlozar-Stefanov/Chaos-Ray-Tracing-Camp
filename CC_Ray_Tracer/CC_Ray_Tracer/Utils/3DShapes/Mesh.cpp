#include "Mesh.h"

bool Mesh::intersects(const Ray& r, Intersection& intersection) const
{
	bool hit = false;
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

		if (triangle.intersects(r, intersection))
		{
			hit = true;
		}
	}
	return hit;
}
