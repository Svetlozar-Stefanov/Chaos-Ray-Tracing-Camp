#pragma once
#include "../Utils/Math/Color.h"
#include "Ray.h"

enum class MaterialType
{
	Diffuse,
	Reflective,
	Refractive,
	Constant
};

class Material
{
protected:
	Color mAlbedo;
	MaterialType mType;
	float mIor;
	bool mIsSmooth;

	public:
	Material(Color albedo, MaterialType type, bool isSmooth, float ior = 1.0f)
	{
		mAlbedo = albedo;
		mType = type;
		mIsSmooth = isSmooth;
		mIor = ior;
	}

	const Color& getAlbedo() const
	{
		return mAlbedo;
	}

	const MaterialType& getType() const
	{
		return mType;
	}

	bool isSmooth() const
	{
		return mIsSmooth;
	}

	float getIOR() const
	{
		return mIor;
	}
};