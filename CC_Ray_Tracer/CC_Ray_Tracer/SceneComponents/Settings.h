#pragma once
#include "../Utils/Math/Color.h"

class Settings
{
public:
	Settings() {
		backgroundColor = Color(0.0, 0.0, 0.0);
		imageWidth = IMAGE_WIDTH;
		imageHeight = IMAGE_HEIGHT;
	}

	Settings(Color bgColor, int imgWidth, int imgHeight) {
		backgroundColor = bgColor;
		imageWidth = imgWidth;
		imageHeight = imgHeight;
	}

	Color backgroundColor;
	int imageWidth;
	int imageHeight;
};