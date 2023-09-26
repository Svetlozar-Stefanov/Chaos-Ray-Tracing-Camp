#pragma once
#include "Renderer.h"

class PNGRenderer : public Renderer
{
	// Inherited via Renderer
	virtual void formatRender(const std::string& fileName, int width, int height, unsigned char* pixels) override;
};

