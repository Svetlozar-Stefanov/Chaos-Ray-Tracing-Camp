#pragma once
#include "../Utils/3DShapes/BoundingBox.h"
#include "../Utils/3DShapes/Triangle.h"
#include <vector>

using std::vector;

struct Node
{
	BoundingBox bBox;
	int lChild = -1;
	int rChild = -1;
	vector<Triangle> triangles;
};

class AABBAccelerator
{
public:
	void createTree(BoundingBox bBox, const vector<Triangle>& triangles);
	bool intersects(const Ray& ray, Intersection& intersection) const;
private:
	int rootIdx = 0;
	int idx = -1;
	vector<Node> nodes;
	void buildTree(int curr, int depth, vector<Triangle> triangles);
	int addNode(BoundingBox bBox, int lChild, int rChild, vector<Triangle> triangles);
	int addNode(Node node);
};