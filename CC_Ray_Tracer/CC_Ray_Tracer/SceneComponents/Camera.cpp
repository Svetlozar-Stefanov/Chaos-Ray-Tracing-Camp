#include "Camera.h"

Ray Camera::generateRay(int pixelX, int pixelY) const
{
	float x = pixelX + 0.5;
	float y = pixelY + 0.5;

	x /= (float)mImageWidth;
	y /= (float)mImageHeight;

	x = (2.0 * x) - 1;
	y = 1 - (2.0 * y);

	x *= (float)mImageWidth / mImageHeight;

	Vector3 rayDir(x, y, -1);

	rayDir *= mRotationMatrix;

	normalize(rayDir);

	return Ray(mOrigin, rayDir, RayType::Camera, 0);
}
