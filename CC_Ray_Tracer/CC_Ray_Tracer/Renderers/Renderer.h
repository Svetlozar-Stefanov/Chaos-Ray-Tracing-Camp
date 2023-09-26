#pragma once
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <string>
#include <mutex>
#include <stack>
#include "../Utils/Math/UtilFunctions.h"
#include "../SceneComponents/Scene.h"

const int MAX_COLOR_COMP = 256;
const int COLOR_CHANNELS = 3;
const float SHADOW_BIAS = 0.1f;
const float REFRACTION_BIAS = 0.001f;
const float MAX_DEPTH = 5;
const int BUCKET_SIZE = 100;

using std::string;
using std::cout;
using std::vector;
using std::ofstream;
using std::stack;
using std::mutex;

struct Bucket
{
	Bucket()
	{
		colIdx = 0;
		rowIdx = 0;
		rHeight = 0;
		rWidth = 0;
	}

	Bucket(int col, int row, int rH, int rW)
	{
		colIdx = col;
		rowIdx = row;
		rHeight = rH;
		rWidth = rW;
	}

	int colIdx;
	int rowIdx;
	int rHeight;
	int rWidth;
};

class Renderer
{
public:
	void render(const Scene& scene, const std::string& fileName);
protected:
	stack<Bucket> generateBuckets(int bucketSize, int width, int height);
	inline void renderBuckets(const Scene& scene, unsigned char* pixels, int width);
	void renderRegion(const Scene& scene, unsigned char* pixels, int colIdx, int rowIdx, int rHeight, int rWidth, int width);
	virtual void formatRender(const std::string& fileName, int width, int height, unsigned char* pixels) = 0;
	Color colorAtPixel(int x, int y, const Scene& scene) const;
	Color traceRay(const Ray& ray, Intersection& intersection, const Scene& scene) const;
	Color shade(const Ray& ray, const Intersection& intersection, const Scene& scene) const;
	Color diffuseShade(const Vector3& interPoint, const Intersection& intersection, const Scene& scene) const;
	Color reflectiveShade(const Ray& ray, const Intersection& intersection, const Scene& scene) const;
	Color refractiveShade(const Ray& ray, const Vector3& interPoint, const Intersection& intersection, const Scene& scene) const;
};

