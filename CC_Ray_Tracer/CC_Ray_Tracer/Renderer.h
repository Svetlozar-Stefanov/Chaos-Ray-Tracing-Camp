#pragma once
#include <string>
#include "Utils/stb_image/stb_image.h"
#include "Utils/stb_image/stb_image_write.h"
#include "Utils/Math/UtilFunctions.h"
#include "SceneComponents/Scene.h"

class Renderer
{
public:
	void renderToPPM(const Scene& scene, const std::string& fileName);

	void renderToJPG(const Scene& scene, const std::string& fileName);

	void renderToPNG(const Scene& scene, const std::string& fileName);
private:
	Color colorAtPixel(int x, int y, const Scene& scene) const;
	Color traceRay(const Ray& ray, Intersection& intersection, const Scene& scene) const;
	Color shade(const Ray& ray, const Intersection& intersection, const Scene& scene) const;
	Color diffuseShade(const Vector3& interPoint, const Intersection& intersection, const Scene& scene) const;
	Color reflectiveShade(const Ray& ray, const Intersection& intersection, const Scene& scene) const;
	Color refractiveShade(const Ray& ray, const Vector3& interPoint, const Intersection& intersection, const Scene& scene) const;
};

