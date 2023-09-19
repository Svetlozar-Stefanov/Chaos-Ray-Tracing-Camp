#pragma once
#include "Renderer.h"

int main()
{
	//Scene scene0("Scenes/scene0.crtscene");
	//Scene scene1("Scenes/scene1.crtscene");
	//Scene scene2("Scenes/scene2.crtscene");
	//Scene scene3("Scenes/scene3.crtscene");
	Scene scene4("Scenes/scene4.crtscene");
	//Scene scene5("Scenes/scene5.crtscene");

	Renderer renderer;
	//renderer.renderToPNG(scene0, "Pictures/HW09/scene0.png");
	//renderer.renderToJPG(scene1, "Pictures/HW09/scene1.jpg");
	//renderer.renderToJPG(scene2, "Pictures/HW09/scene2.jpg");
	//renderer.renderToJPG(scene3, "Pictures/HW09/scene3.jpg");
	renderer.renderToJPG(scene4, "Pictures/HW09/scene4.jpg");
	//renderer.renderToJPG(scene5, "Pictures/HW09/scene5.jpg");
}