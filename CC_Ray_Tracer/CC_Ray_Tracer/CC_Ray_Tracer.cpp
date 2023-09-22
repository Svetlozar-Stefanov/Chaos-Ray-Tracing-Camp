#pragma once
#include "Renderer.h"

int main()
{
	Scene scene0("Scenes/HW_11/scene0.crtscene");
	Scene scene1("Scenes/HW_11/scene1.crtscene");
	Scene scene2("Scenes/HW_11/scene2.crtscene");
	Scene scene3("Scenes/HW_11/scene3.crtscene");
	Scene scene4("Scenes/HW_11/scene4.crtscene");
	//scene scene5("Scenes/HW_11/scene5.crtscene");
	Scene scene6("Scenes/HW_11/scene6.crtscene");
	Scene scene7("Scenes/HW_11/scene7.crtscene");
	Scene scene8("Scenes/HW_11/scene8.crtscene");

	Renderer renderer;
	renderer.renderToPNG(scene0, "Pictures/HW11/scene0.png");
	renderer.renderToPNG(scene1, "Pictures/HW11/scene1.png");
	renderer.renderToPNG(scene2, "Pictures/HW11/scene2.png");
	renderer.renderToPNG(scene3, "Pictures/HW11/scene3.png");
	renderer.renderToPNG(scene4, "Pictures/HW11/scene4.png");
	//renderer.renderToPNG(scene5, "Pictures/HW11/scene5.png");
	renderer.renderToPNG(scene6, "Pictures/HW11/scene6.png");
	renderer.renderToPNG(scene7, "Pictures/HW11/scene7.png");
	renderer.renderToPNG(scene8, "Pictures/HW11/scene8.png");
}