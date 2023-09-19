#pragma once
#include "../Utils/Math/Vector3.h"
#include "../Utils/Math/Matrix.h"
#include "Ray.h"

const int IMAGE_WIDTH = 1280;
const int IMAGE_HEIGHT = 720;

class Camera
{
private:
	Vector3 mOrigin;
	Matrix mRotationMatrix;

	int mImageWidth;
	int mImageHeight;

public:
	Camera()
	{
		mOrigin = Vector3(0,0,0);
		mImageWidth = IMAGE_WIDTH;
		mImageHeight = IMAGE_HEIGHT;
	}

	Camera(Vector3 origin, int imageWidth = IMAGE_WIDTH, int imageHeight = IMAGE_HEIGHT)
	{
		mOrigin = origin;
		mImageWidth = imageWidth;
		mImageHeight = imageHeight;
	}

	Camera(Vector3 origin, Matrix rotationMatrix, int imageWidth = IMAGE_WIDTH, int imageHeight = IMAGE_HEIGHT)
	{
		mOrigin = origin;
		mRotationMatrix = rotationMatrix;
		mImageWidth = imageWidth;
		mImageHeight = imageHeight;
	}

	void move(const Vector3& dir, float distance)
	{
		Vector3 dirAfterRot = dir * mRotationMatrix;
		mOrigin += dirAfterRot * distance;
	}

	void pan(float deg)
	{
		mRotationMatrix *= rotateY(deg);
	}

	void tilt(float deg)
	{
		mRotationMatrix *= rotateX(deg);
	}

	void roll(float deg)
	{
		mRotationMatrix *= rotateZ(deg);
	}

	void setImageWidth(int imageWidth)
	{
		mImageWidth = imageWidth;
	}

	void setImageHeight(int imageHeight)
	{
		mImageHeight = imageHeight;
	}

	void setRotationMatrix(const Matrix& rotationMatrix)
	{
		mRotationMatrix = rotationMatrix;
	}

	void setOrigin(const Vector3& origin)
	{
		mOrigin = origin;
	}

	Ray generateRay(int pixelX, int pixelY) const;
};

