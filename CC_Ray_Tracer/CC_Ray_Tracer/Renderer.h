#pragma once
#include "Utils/stb_image/stb_image.h"

#include "SceneComponents/Scene.h"

using std::vector;
using std::ofstream;

class Renderer
{
public:
	void renderToPPM(const Scene& scene, const string& fileName);

	void renderToJPG(const Scene& scene, const string& fileName);
};

