#pragma once
#include "Renderer.h"

int main()
{
	Scene scene0("Scenes/scene0.crtscene");
	Scene scene1("Scenes/scene1.crtscene");
	Scene scene2("Scenes/scene2.crtscene");
	Scene scene3("Scenes/scene3.crtscene");

	Renderer renderer;
	//renderer.renderToJPG(scene0, "scene0.jpg");
	renderer.renderToJPG(scene1, "scene1.jpg");
	//renderer.renderToPPM(scene2, "scene2.ppm");
	//renderer.renderToPPM(scene3, "scene3.ppm");
}