#pragma once
#include <limits>
#include "../Utils/Math/Vector3.h"
#include "Material.h"

class Intersection
{
private:
	float mT;
	const Material* mMaterial;
	Vector3 mNormal;
	Vector3 mIntersectionNormal;

public:
	Intersection()
	{
		mT = std::numeric_limits<float>::max();
		mMaterial = nullptr;
	}

	Intersection(float t)
	{
		mT = t;
		mMaterial = nullptr;
	}

	float getT() const
	{
		return mT;
	}

	const Material* const getMaterial() const
	{
		return mMaterial;
	}

	const Vector3& getNormal() const
	{
		return mNormal;
	}

	const Vector3& getIntersectionNormal() const
	{
		return mIntersectionNormal;
	}

	void updateMaterial(const Material* material)
	{
		mMaterial = material;
	}

	void update(float t, const Material* material, Vector3 normal, Vector3 intersectionNormal)
	{
		mT = t;
		mMaterial = material;
		mNormal = normal;
	}

	void update(float t, Vector3 normal, Vector3 intersectionNormal)
	{
		mT = t;
		mNormal = normal;
		mIntersectionNormal = intersectionNormal;
	}
};

enum class RayType
{
	Camera,
	Shadow,
	Reflection,
	Refraction
};

class Ray
{
	Vector3 mOrigin;
	Vector3 mDirection;
	RayType mType;
	int mDepth;

public:
	Ray(Vector3 origin, Vector3 direction, RayType type, int depth) {
		mOrigin = origin;
		mDirection = direction;
		mType = type;
		mDepth = depth;
	}

	Vector3 getOrigin() const
	{
		return mOrigin;
	}
	
	Vector3 getDirection() const {
		return mDirection;
	}

	Vector3 getPoint(float t) const
	{
		return mOrigin + mDirection * t;
	}

	RayType getType() const
	{
		return mType;
	}

	int getDepth() const
	{
		return mDepth;
	}
};