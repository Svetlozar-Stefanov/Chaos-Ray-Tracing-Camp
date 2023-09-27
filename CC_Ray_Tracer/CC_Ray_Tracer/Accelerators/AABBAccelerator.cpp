#include "AABBAccelerator.h"
#include <stack>

using std::stack;

const int MAX_DEPTH = 20;
const int MAX_TRIANGLES_PER_NODE = 10;

void AABBAccelerator::createTree(BoundingBox bBox, const vector<Triangle>& triangles)
{
	rootIdx = addNode(bBox, -1, -1, triangles);

	buildTree(rootIdx, 0, triangles);
}

void AABBAccelerator::buildTree(int curr, int depth, vector<Triangle> triangles)
{
	if (depth >= MAX_DEPTH || triangles.size() <= MAX_TRIANGLES_PER_NODE)
	{
		nodes[curr].triangles = triangles;
		return;
	}

	Node left;
	Node right;
	nodes[curr].bBox.split(depth % 3, left.bBox, right.bBox);

	for (auto& triangle : triangles)
	{
		if (left.bBox.boxIntersect(triangle.getBBox()))
		{
			left.triangles.push_back(triangle);
		}
		if (right.bBox.boxIntersect(triangle.getBBox()))
		{
			right.triangles.push_back(triangle);
		}
	}

	if (left.triangles.size() > 0)
	{
		nodes[curr].lChild = addNode(left);
		buildTree(nodes[curr].lChild, depth + 1, left.triangles);
	}
	if (right.triangles.size() > 0)
	{
		nodes[curr].rChild = addNode(right);
		buildTree(nodes[curr].rChild, depth + 1, right.triangles);
	}

	nodes[curr].triangles.clear();
}

bool AABBAccelerator::intersects(const Ray& ray, Intersection& intersection) const
{
	stack<int> toCheck;
	toCheck.push(rootIdx);
	bool hit = false;
	while (!toCheck.empty())
	{
		int curr = toCheck.top();
		toCheck.pop();
		const Node& current = nodes[curr];
		if (current.bBox.intersects(ray))
		{
			if (current.triangles.size() > 0)
			{
				for (const auto& triangle : current.triangles)
				{
					if (triangle.intersects(ray, intersection))
					{
						hit = true;
					}
				}
			}
			else
			{
				if (current.lChild != -1)
				{
					toCheck.push(current.lChild);
				}
				if(current.rChild != -1)
				{
					toCheck.push(current.rChild);
				}
			}
		}
	}
	return hit;
}

int AABBAccelerator::addNode(BoundingBox bBox, int lChild, int rChild, vector<Triangle> triangles)
{
	nodes.push_back(Node({ bBox, lChild, rChild, triangles }));
	return ++idx;
}

int AABBAccelerator::addNode(Node node)
{
	nodes.push_back(node);
	return ++idx;
}